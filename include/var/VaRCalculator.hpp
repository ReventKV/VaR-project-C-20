#pragma once

#include "var/Portfolio.hpp"

namespace var {

/**
 * @brief Абстрактный интерфейс стратегии расчета Value at Risk (VaR).
 * 
 * Определяет единый контракт для всех алгоритмов оценки риска (Параметрический, 
 * Монте-Карло, Исторический). Паттерн "Strategy".
 */
class VaRCalculator {
public:
    virtual ~VaRCalculator() = default;

    /**
     * @brief Вычисляет величину риска (VaR) в валюте портфеля.
     * 
     * @param portfolio Портфель для оценки.
     * @param confidence Уровень доверия (например, 0.95 для 95% или 0.99 для 99%).
     * @param horizon Горизонт инвестирования в днях.
     * @return Положительное число, представляющее максимальный ожидаемый убыток.
     */
    virtual double calculateVaR(const Portfolio& portfolio, double confidence, int horizon) const = 0;
};
}