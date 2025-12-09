// include/var/ParametricVaRCalculator.hpp
#pragma once

#include "var/VaRCalculator.hpp"
#include "var/RiskModel.hpp"
#include <map>
#include <string>

namespace var {

/**
 * @brief Калькулятор параметрического VaR (Delta-Normal / Variance-Covariance).
 * 
 * Предполагает нормальное распределение доходностей активов.
 * Использует аналитическую формулу:
 * \f$ VaR = Value \times z_{\alpha} \times \sigma_p \times \sqrt{T} \f$
 */
class ParametricVaRCalculator : public VaRCalculator {
public:

    /**
     * @brief Конструктор.
     * @param marketData Карта рыночных данных ("Тикер" -> "История цен").
     */
    ParametricVaRCalculator(std::map<std::string, TimeSeries> marketData);

    /**
     * @brief Вычисляет VaR аналитически.
     * 
     * 1. Строит ковариационную матрицу активов портфеля.
     * 2. Вычисляет дисперсию портфеля на основе весов позиций.
     * 3. Масштабирует стандартное отклонение на Z-score (квантиль нормального распределения).
     */
    double calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const override;

private:
    std::map<std::string, TimeSeries> marketData_;
    // Нужно ли ?
    RiskModel riskModel_;
    
    /**
     * @brief Возвращает квантиль нормального распределения (Z-score).
     * Например, 1.645 для 95%.
     */
    double getNormalQuantile(double confidence) const;
};
}