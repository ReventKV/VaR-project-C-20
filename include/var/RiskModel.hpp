#pragma once

#include "var/TimeSeries.hpp"
#include <vector>
#include <Eigen/Dense>

namespace var {

/**
 * @brief Математическое ядро моделирования рисков.
 * 
 * Отвечает за обработку сырых временных рядов, их выравнивание и 
 * построение ковариационной матрицы.
 */
class RiskModel {
public:
    /**
     * @brief Вычисляет ковариационную матрицу доходностей.
     * 
     * Метод автоматически выполняет выравнивание данных (alignTimeSeries),
     * расчет доходностей и построение матрицы.
     * 
     * @param timeSeriesList Вектор временных рядов (по одному на каждый актив).
     * @return Квадратная матрица Eigen (N x N).
     * @throw std::runtime_error Если данных недостаточно или они некорректны.
     */
    Eigen::MatrixXd computeCovarianceMatrix(const std::vector<TimeSeries>& timeSeriesList);
    
    /**
     * @brief Выравнивает временные ряды по единой шкале дат.
     * 
     * Статический метод.
     * 1. Собирает объединение (Union) всех дат из всех активов.
     * 2. Если у актива нет цены на дату T, используется цена T-1 (Forward Fill).
     * Это критически важно для корректного расчета корреляций между активами с разных бирж.
     * 
     * @param inputs Исходные временные ряды.
     * @return Вектор выровненных рядов (все имеют одинаковую длину и набор дат).
     */
    static std::vector<TimeSeries> alignTimeSeries(const std::vector<TimeSeries>& inputs);
};
}