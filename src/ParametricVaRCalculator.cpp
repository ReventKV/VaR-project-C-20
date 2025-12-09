#include "var/ParametricVaRCalculator.hpp"
#include "var/ReturnCalculator.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
namespace var {
ParametricVaRCalculator::ParametricVaRCalculator(std::map<std::string, TimeSeries> marketData)
    : marketData_(std::move(marketData)) {}

double ParametricVaRCalculator::calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const {

    auto positions = portfolio.getPositions();
    if (positions.empty()) return 0.0;

    std::vector<std::string> activeTickers;
    std::vector<double> weights;
    double totalValue = portfolio.getTotalValue();


    for (const auto& pos : positions) {
        std::string id = pos.getInstrument()->getId();
        

        if (marketData_.find(id) == marketData_.end()) {
            throw std::runtime_error("Missing market data for asset: " + id);
        }

        activeTickers.push_back(id);
        weights.push_back(pos.getValue() / totalValue);
    }


    std::vector<TimeSeries> activeSeries;
    for (const auto& id : activeTickers) {
        activeSeries.push_back(marketData_.at(id));
    }


    RiskModel localModel; 
    Eigen::MatrixXd covMatrix = localModel.computeCovarianceMatrix(activeSeries);


    Eigen::VectorXd w(weights.size());
    for (size_t i = 0; i < weights.size(); ++i) {
        w(i) = weights[i];
    }


    double portVariance = w.transpose() * covMatrix * w;
    double portStdDev = std::sqrt(portVariance);


    double zScore = getNormalQuantile(confidence);


    double vaR = totalValue * zScore * portStdDev * std::sqrt(horizon);

    return vaR;
}

// Простая аппроксимация обратной функции нормального распределения
// (Beasley-Springer-Moro algorithm или простая аппроксимация для 95/99%)
double ParametricVaRCalculator::getNormalQuantile(double confidence) const {

    if (std::abs(confidence - 0.95) < 0.001) return 1.645;
    if (std::abs(confidence - 0.99) < 0.001) return 2.326;
    
    return 1.645; 
}
}