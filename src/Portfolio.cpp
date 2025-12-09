#include "var/Portfolio.hpp"

namespace var {
    
void Portfolio::addPosition(const Position& pos) {
    positions_.push_back(pos);
}

double Portfolio::getTotalValue() const {
    double total = 0.0;
    for (const auto& pos : positions_) {
        total += pos.getValue();
    }
    return total;
}

const std::vector<Position>& Portfolio::getPositions() const {
    return positions_;
}
}