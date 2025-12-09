
#include "var/ReturnCalculator.hpp"
#include <cmath>
#include <stdexcept>
namespace var {
std::vector<double> ReturnCalculator::computeLogReturns(const TimeSeries& ts) {
    const auto& records = ts.getRecords();
    if (records.size() < 2) {
        return {};
    }

    std::vector<double> returns;
    returns.reserve(records.size() - 1);

    for (size_t i = 1; i < records.size(); ++i) {
        double prev = records[i - 1].price;
        double curr = records[i].price;

        if (prev <= 0 || curr <= 0) {
            throw std::runtime_error("Prices must be positive for log returns");
        }

        returns.push_back(std::log(curr / prev));
    }

    return returns;
}
}