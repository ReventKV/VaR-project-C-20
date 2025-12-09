#include "var/MonteCarloVaRCalculator.hpp"
#include "var/ValuationFunction.hpp"
#include "var/ReturnCalculator.hpp"
#include "var/MCStatistics.hpp"
#include <vector>
#include <iostream>

namespace var {

MonteCarloVaRCalculator::MonteCarloVaRCalculator(int numPaths, std::map<std::string, TimeSeries> marketData, uint32_t seed)
    : numPaths_(numPaths), marketData_(std::move(marketData)), seed_(seed) {}

double MonteCarloVaRCalculator::calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const {
    auto positions = portfolio.getPositions();
    if (positions.empty()) return 0.0;

    std::vector<TimeSeries> activeSeries;
    std::vector<std::shared_ptr<ValuationFunction>> valuators;
    double currentTotalValue = portfolio.getTotalValue();
    size_t idx = 0;

    for (const auto& pos : positions) {
        std::string id = pos.getInstrument()->getId();
        if (marketData_.find(id) == marketData_.end()) {
            throw std::runtime_error("No market data for: " + id);
        }
        activeSeries.push_back(marketData_.at(id));
        valuators.push_back(std::make_shared<StockValuation>(
            pos.getInstrument()->getPrice(),
            pos.getQuantity(),
            idx
        ));
        idx++;
    }

    RiskModel riskModel;
    Eigen::MatrixXd cov = riskModel.computeCovarianceMatrix(activeSeries);

    double dailyRiskFree = 0.0; 
    

    MonteCarloEngine engine(cov, dailyRiskFree, seed_); 
    
    auto scenarios = engine.simulate(numPaths_, horizon);

    MCStatistics stats;
    for (const auto& scen : scenarios) {
        double scenarioValue = 0.0;
        for (const auto& val : valuators) {
            scenarioValue += val->value(scen);
        }
        double pnl = scenarioValue - currentTotalValue;
        stats.addResult(pnl);
    }

    return stats.computeVaR(confidence);
}

}