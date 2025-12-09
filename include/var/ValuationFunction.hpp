#pragma once

#include "var/SimulationEngine.hpp"
#include "var/Instrument.hpp" 
#include <cmath>


namespace var {

/**
 * @brief Абстрактная функция оценки стоимости (Valuation).
 * 
 * Паттерн "Strategy". Определяет, как изменится стоимость конкретной позиции
 * при заданном рыночном сценарии.
 */
class ValuationFunction {
public:
    virtual ~ValuationFunction() = default;

    /**
     * @brief Возвращает новую стоимость позиции в сценарии.
     * @param scenario Вектор доходностей рынка.
     */
    virtual double value(const Scenario& scenario) const = 0;
};

class StockValuation : public ValuationFunction {
public:
    
    /**
    * @brief Оценка для простых акций (Linear/Log-Normal payoff).
    * 
    * Модель: \f$ P_{new} = P_{old} \times \exp(r) \times Quantity \f$
    */
    StockValuation(double currentPrice, double quantity, size_t idx)
        : price_(currentPrice), quantity_(quantity), idx_(idx) {}

    double value(const Scenario& scenario) const override {
        // scenario[idx_] - это логарифмическая доходность фактора риска
        double logReturn = scenario.at(idx_);
        double newPrice = price_ * std::exp(logReturn);
        return newPrice * quantity_;
    }

private:
    double price_;
    double quantity_;
    size_t idx_;
};
}