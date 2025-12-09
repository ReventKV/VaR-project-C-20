#pragma once

#include "var/Instrument.hpp"
#include <vector>


namespace var {

/**
 * @brief Инвестиционный портфель.
 * 
 * Коллекция позиций. Предоставляет методы для агрегированной оценки стоимости.
 */
class Portfolio {
public:
    /**
     * @brief Добавляет позицию в портфель.
     * @param pos Объект позиции.
     */
    void addPosition(const Position& pos);

   /**
     * @brief Считает полную стоимость портфеля (Net Asset Value).
     * Суммирует стоимость всех входящих позиций по текущим ценам.
     * @return Общая стоимость в валюте активов.
     */
    double getTotalValue() const;

    /**
     * @brief Предоставляет доступ к списку позиций (только для чтения).
     * Необходим калькуляторам риска для итерации по активам.
     */
    const std::vector<Position>& getPositions() const;

private:
    std::vector<Position> positions_;
};

}