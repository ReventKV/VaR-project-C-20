#include "var/MCStatistics.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace var {

void MCStatistics::addResult(double pnl) {
    results_.push_back(pnl);
}

double MCStatistics::computeVaR(double confidence) {
    if (results_.empty()) return 0.0;


    size_t index = static_cast<size_t>(results_.size() * (1.0 - confidence));
    if (index >= results_.size()) index = results_.size() - 1;


    std::nth_element(results_.begin(), 
                     results_.begin() + index, 
                     results_.end());

    double varValue = results_[index];


    return (varValue < 0) ? -varValue : 0.0;
}

}