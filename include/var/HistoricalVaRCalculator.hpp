#pragma once

#include "var/VaRCalculator.hpp"
#include "var/TimeSeries.hpp"
#include <map>
#include <string>

namespace var {

/**
 * @brief Калькулятор Исторического VaR (Historical Simulation).
 * 
 * Не делает предположений о распределении доходностей.
 * "Проигрывает" реальные исторические изменения цен на текущем портфеле.
 * Использует алгоритм выравнивания дат (Data Alignment).
 */
class HistoricalVaRCalculator : public VaRCalculator {
public:
    HistoricalVaRCalculator(std::map<std::string, TimeSeries> marketData);

    /**
     * @brief Вычисляет VaR.
     * 
     * 1. Выравнивает ряды данных.
     * 2. Рассчитывает исторические P&L для каждого дня в прошлом.
     * 3. Находит эмпирический квантиль убытков.
     * 4. Масштабирует результат на горизонт по правилу корня из времени.
     */
    double calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const override;

private:
    std::map<std::string, TimeSeries> marketData_;
};

}