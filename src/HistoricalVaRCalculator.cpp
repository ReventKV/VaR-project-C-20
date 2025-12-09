#include "var/HistoricalVaRCalculator.hpp"
#include "var/RiskModel.hpp"
#include "var/ReturnCalculator.hpp"
#include "var/MCStatistics.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace var {

HistoricalVaRCalculator::HistoricalVaRCalculator(std::map<std::string, TimeSeries> marketData)
    : marketData_(std::move(marketData)) {}

double HistoricalVaRCalculator::calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const {
    auto positions = portfolio.getPositions();
    if (positions.empty()) return 0.0;

    std::vector<TimeSeries> rawSeries;
    std::vector<double> positionValues;
    
    for (const auto& pos : positions) {
        std::string id = pos.getInstrument()->getId();
        if (marketData_.find(id) == marketData_.end()) {
            throw std::runtime_error("Missing market data for: " + id);
        }
        rawSeries.push_back(marketData_.at(id));
        positionValues.push_back(pos.getValue());
    }

    std::vector<TimeSeries> alignedSeries = RiskModel::alignTimeSeries(rawSeries);
    if (alignedSeries.empty() || alignedSeries[0].size() < 2) {
        return 0.0;
    }

    std::vector<std::vector<double>> assetReturns;
    size_t numDays = 0;

    for (const auto& ts : alignedSeries) {
        auto rets = ReturnCalculator::computeLogReturns(ts);
        if (numDays == 0) numDays = rets.size();
        else numDays = std::min(numDays, rets.size());
        assetReturns.push_back(rets);
    }


    MCStatistics stats;


    for (size_t day = 0; day < numDays; ++day) {
        double dailyPnL = 0.0;

        for (size_t i = 0; i < positions.size(); ++i) {
            double logRet = assetReturns[i][day];
            

            double assetPnL = positionValues[i] * (std::exp(logRet) - 1.0);
            dailyPnL += assetPnL;
        }

        stats.addResult(dailyPnL);
    }

    double oneDayVaR = stats.computeVaR(confidence);


    return oneDayVaR * std::sqrt(horizon);
}

} 