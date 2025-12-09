// include/var/ReturnCalculator.hpp
#pragma once

#include "var/TimeSeries.hpp"
#include <vector>
namespace var {

/**
 * @brief Утилита для расчета доходностей.
 * 
 * Содержит статические методы для преобразования цен в доходности.
 */
class ReturnCalculator {
public:

    /**
     * @brief Вычисляет логарифмические доходности.
     * 
     * Formula: r_i = ln(P_i / P_{i-1})
     * Лог-доходности предпочтительны для моделирования, так как они аддитивны во времени.
     * 
     * @param ts Временной ряд цен.
     * @return Вектор доходностей (размером N-1).
     * @throw std::runtime_error Если цены <= 0 или данных недостаточно.
     */
    static std::vector<double> computeLogReturns(const TimeSeries& ts);
};
}