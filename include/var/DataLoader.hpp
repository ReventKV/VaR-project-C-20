#pragma once

#include "var/TimeSeries.hpp"
#include <string>
#include <vector>

namespace var {

/**
 * @brief Парсер CSV строк.
 * 
 * Низкоуровневый класс, отвечающий за разделение строки на токены
 * по заданному разделителю.
 */
class CsvReader {
public:

    /**
     * @brief Конструктор.
     * @param delimiter Символ-разделитель (по умолчанию ',').
     */
    CsvReader(char delimiter = ',') : delimiter_(delimiter) {}

    /**
     * @brief Разбирает строку CSV.
     * @param line Входящая строка (например, "2023-01-01,100.50").
     * @return Вектор строк-токенов.
     */
    std::vector<std::string> parseLine(const std::string& line) const;

private:
    char delimiter_;
};

/**
 * @brief Загрузчик исторических данных из файлов.
 * 
 * Фасад для чтения файлов. Открывает файл, использует CsvReader для парсинга
 * и формирует объект TimeSeries.
 */
class DataLoader {
public:

    /**
     * @brief Конструктор.
     * @param filePath Полный или относительный путь к CSV файлу.
     */
    DataLoader(const std::string& filePath) : filePath_(filePath) {}
    
    /**
     * @brief Загружает данные из файла.
     * 
     * Метод ожидает, что 1-я колонка - это Дата, а 2-я - Цена.
     * Ошибочные строки пропускаются.
     * 
     * @return Заполненный объект TimeSeries.
     * @throw std::runtime_error Если файл не удалось открыть.
     */
    TimeSeries load();

private:
    std::string filePath_;
};
}