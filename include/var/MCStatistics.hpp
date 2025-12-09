#pragma once

#include <vector>

namespace var {

/**
 * @brief Сборщик статистики симуляций.
 * 
 * Агрегирует результаты (P&L) всех путей Монте-Карло или Исторического метода
 * и вычисляет итоговый VaR.
 */
class MCStatistics {
public:

    /**
     * @brief Добавить результат одного прогона (сценария).
     * @param pnl Прибыль или убыток (Profit & Loss).
     */
    void addResult(double pnl);

    /**
     * @brief Вычисляет VaR на основе распределения убытков.
     * 
     * Использует оптимизированный алгоритм std::nth_element (O(N)) вместо полной сортировки.
     * 
     * @param confidence Уровень доверия (0.0 - 1.0).
     * @return Положительное значение VaR.
     */
    double computeVaR(double confidence);

    /// @brief Количество собранных сценариев.
    size_t size() const { return results_.size(); }

private:
    std::vector<double> results_;
};
}