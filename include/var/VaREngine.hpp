#pragma once

#include "var/Portfolio.hpp"
#include "var/TimeSeries.hpp"
#include <map>
#include <string>


namespace var {

/**
 * @brief Главный фасад (Orchestrator) библиотеки.
 * 
 * Предоставляет высокоуровневый API для управления портфелем и запуска расчетов.
 * Скрывает сложность инициализации отдельных калькуляторов.
 */
class VaREngine {
public:
    
    /**
     * @brief Добавить актив в анализ.
     * 
     * Читает CSV, создает инструмент и добавляет позицию.
     * Выводит лог загрузки в консоль.
     * 
     * @param ticker Тикер актива.
     * @param csvPath Путь к файлу данных.
     * @param quantity Количество.
     * @param currentPrice Текущая цена.
     */
    void addAsset(const std::string& ticker, const std::string& csvPath, double quantity, double currentPrice);

    /**
     * @brief Запуск полного отчета о рисках.
     * 
     * Последовательно запускает Parametric, Historical и Monte Carlo калькуляторы
     * и выводит сравнительную таблицу результатов в stdout.
     * 
     * @param confidence Уровень доверия (например, 0.95).
     * @param horizonDays Горизонт (дней).
     * @param mcPaths Число путей Монте-Карло (по умолчанию 10000).
     */
    void runReport(double confidence, int horizonDays, int mcPaths = 10000);

private:
    Portfolio portfolio_;
    std::map<std::string, TimeSeries> marketData_;
};
}