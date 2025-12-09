#pragma once

#include <string>
#include <vector>
#include <stdexcept>

// Одна точка данных: дата и цена
namespace var {

/**
 * @brief Единичная запись исторических данных.
 * 
 * Структура хранит дату (в строковом формате) и цену закрытия (или иную цену)
 * на эту дату.
 */
struct PriceRecord {
    std::string date;
    double price;
};

/**
 * @brief Временной ряд (история цен).
 * 
 * Класс-контейнер для хранения хронологической последовательности цен одного актива.
 * Обеспечивает доступ к данным и вспомогательные методы конвертации.
 */
class TimeSeries {
public:

    /**
     * @brief Добавляет новую запись в конец ряда.
     * @param record Заполненная структура PriceRecord.
     */
    void add(const PriceRecord& record) {
        records_.push_back(record);
    }

    /**
     * @brief Возвращает вектор цен (без дат).
     * 
     * Удобно для математических вычислений, где требуются только числовые ряды.
     * @return std::vector<double> Список цен в хронологическом порядке.
     */
    std::vector<double> getPriceSeries() const {
        std::vector<double> prices;
        prices.reserve(records_.size());
        for (const auto& rec : records_) {
            prices.push_back(rec.price);
        }
        return prices;
    }

    /// @brief Возвращает ссылку на внутренний вектор записей.
    const std::vector<PriceRecord>& getRecords() const {
        return records_;
    }

    /// @brief Возвращает количество записей в ряду.
    size_t size() const {
        return records_.size();
    }

private:
    std::vector<PriceRecord> records_;
};

}