#include "var/RiskModel.hpp"
#include "var/ReturnCalculator.hpp"
#include <stdexcept>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>

namespace var {


std::vector<TimeSeries> RiskModel::alignTimeSeries(const std::vector<TimeSeries>& inputs) {
    if (inputs.empty()) return {};


    std::set<std::string> allDates;
    for (const auto& ts : inputs) {
        for (const auto& rec : ts.getRecords()) {
            allDates.insert(rec.date);
        }
    }

    if (allDates.empty()) return {};


    std::vector<std::map<std::string, double>> maps(inputs.size());
    for (size_t i = 0; i < inputs.size(); ++i) {
        for (const auto& rec : inputs[i].getRecords()) {
            maps[i][rec.date] = rec.price;
        }
    }


    std::vector<TimeSeries> alignedData(inputs.size());
    

    std::vector<double> lastKnownPrices(inputs.size(), 0.0);
    

    for (size_t i = 0; i < inputs.size(); ++i) {
        if (!inputs[i].getRecords().empty()) {
            lastKnownPrices[i] = inputs[i].getRecords()[0].price;
        }
    }


    for (const auto& date : allDates) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            auto it = maps[i].find(date);
            double price = 0.0;

            if (it != maps[i].end()) {
                price = it->second;
                lastKnownPrices[i] = price;
            } else {
                price = lastKnownPrices[i];
            }
            alignedData[i].add({date, price});
        }
    }

    return alignedData;
}



Eigen::MatrixXd RiskModel::computeCovarianceMatrix(const std::vector<TimeSeries>& timeSeriesList) {
    if (timeSeriesList.empty()) {
        throw std::invalid_argument("No time series provided");
    }


    std::vector<TimeSeries> alignedSeries = RiskModel::alignTimeSeries(timeSeriesList);

    size_t numAssets = alignedSeries.size();
    

    std::vector<std::vector<double>> allReturns;
    size_t minLen = -1; 

    for (const auto& ts : alignedSeries) {
        auto rets = ReturnCalculator::computeLogReturns(ts);
        
        if (rets.empty()) {
             throw std::runtime_error("Insufficient data after alignment");
        }
        
        if (minLen == (size_t)-1 || rets.size() < minLen) {
            minLen = rets.size();
        }
        allReturns.push_back(rets);
    }

    size_t numObs = minLen; 
    
    if (numObs < 2) {
        throw std::runtime_error("Not enough observations to compute covariance");
    }


    Eigen::MatrixXd mat(numObs, numAssets);

    for (size_t j = 0; j < numAssets; ++j) {
        for (size_t i = 0; i < numObs; ++i) {
            size_t offset = allReturns[j].size() - numObs;
            mat(i, j) = allReturns[j][i + offset];
        }
    }


    Eigen::RowVectorXd means = mat.colwise().mean();
    Eigen::MatrixXd centered = mat.rowwise() - means;
    Eigen::MatrixXd cov = (centered.transpose() * centered) / double(numObs - 1);

    return cov;
}

} 