#pragma once

#include <string>
#include <memory>
#include <stdexcept>


namespace var {

/**
 * @brief Финансовый инструмент (Актив).
 * 
 * Базовая сущность, представляющая торгуемый актив (акцию, облигацию, товар, валюту).
 * Хранит идентификатор и текущую рыночную цену.
 */
class Instrument {
public:
    
    /**
     * @brief Создает новый инструмент.
     * @param id Уникальный тикер (например, "AAPL").
     * @param price Текущая цена (должна быть неотрицательной).
     */
    Instrument(std::string id, double price) 
        : id_(std::move(id)), price_(price) {}

    virtual ~Instrument() = default;

    /**
     * @brief Обновляет рыночную цену.
     * 
     * Изменение цены здесь автоматически повлияет на оценку всех позиций,
     * ссылающихся на этот инструмент.
     * @param newPrice Новая цена.
     * @throw std::invalid_argument Если цена < 0.
     */
    void setPrice(double newPrice) {
        if (newPrice < 0) throw std::invalid_argument("Price cannot be negative");
        price_ = newPrice;
    }

    /// @brief Получить текущую цену.
    double getPrice() const { return price_; }
    
    /// @brief Получить идентификатор (тикер).
    const std::string& getId() const { return id_; }

private:
    std::string id_;
    double price_;
};

/**
 * @brief Позиция в портфеле.
 * 
 * Связывает инструмент и объем владения (количество).
 * Использует shared_ptr для связи с инструментом.
 */
class Position {
public:
    
    /**
     * @brief Создает позицию.
     * @param instrument Умный указатель на инструмент (не может быть nullptr).
     * @param quantity Количество единиц (лотов). Может быть дробным.
     * @throw std::invalid_argument Если instrument == nullptr.
     */
    Position(std::shared_ptr<Instrument> instrument, double quantity)
        : instrument_(std::move(instrument)), quantity_(quantity) {
        
        if (!instrument_) {
            throw std::invalid_argument("Instrument cannot be null");
        }
    }

    /// @brief Возвращает количество.
    double getQuantity() const { return quantity_; }
    
    /**
     * @brief Вычисляет текущую стоимость позиции.
     * Formula: MarketPrice * Quantity
     */
    double getValue() const {
        return instrument_->getPrice() * quantity_;
    }

    /// @brief Возвращает указатель на инструмент.
    std::shared_ptr<Instrument> getInstrument() const {
        return instrument_;
    }

private:
    std::shared_ptr<Instrument> instrument_;
    double quantity_;
};
}