#pragma once

#include "var/VaRCalculator.hpp"
#include "var/RiskModel.hpp"
#include "var/SimulationEngine.hpp"
#include <map>
#include <string>

namespace var {

/**
 * @brief Калькулятор VaR методом Монте-Карло.
 * 
 * Реализует полный цикл стохастического моделирования:
 * 1. Построение модели ковариаций.
 * 2. Параллельная генерация сценариев (GBM).
 * 3. Переоценка портфеля (Valuation).
 * 4. Сбор статистики и расчет квантиля.
 */
class MonteCarloVaRCalculator : public VaRCalculator {
public:

    /**
     * @brief Конструктор.
     * @param numPaths Количество сценариев (рекомендуется >= 10,000).
     * @param marketData Рыночные данные.
     * @param seed Фиксированное зерно RNG (по умолчанию 42).
     */
    MonteCarloVaRCalculator(int numPaths, std::map<std::string, TimeSeries> marketData, uint32_t seed = 42);

    double calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const override;

private:
    int numPaths_;
    std::map<std::string, TimeSeries> marketData_;
    uint32_t seed_;
};

}