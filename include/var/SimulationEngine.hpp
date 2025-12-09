#pragma once

#include <vector>
#include <Eigen/Dense>
#include <random>

namespace var {

/// Вектор доходностей для одного сценария (размер = числу активов).
using Scenario = std::vector<double>;

/**
 * @brief Абстрактный генератор рыночных сценариев.
 */
class SimulationEngine {
public:
    virtual ~SimulationEngine() = default;
    
    /**
     * @brief Генерирует набор сценариев развития рынка.
     * @param nPaths Количество путей симуляции.
     * @param horizonDays Горизонт прогноза в днях.
     * @return Вектор сценариев (каждый сценарий — вектор доходностей активов).
     */
    virtual std::vector<Scenario> simulate(int nPaths, int horizonDays) = 0;
};

/**
 * @brief Движок Монте-Карло на основе Геометрического Броуновского Движения (GBM).
 * 
 * Моделирует коррелированные случайные процессы для активов.
 * Использует разложение Холецкого для корреляции и std::async для многопоточности.
 */
class MonteCarloEngine : public SimulationEngine {
public:

    /**
     * @brief Конструктор.
     * @param covMatrix Ковариационная матрица доходностей.
     * @param riskFreeRate Безрисковая ставка (дневная). По умолчанию 0.
     * @param seed Зерно генератора случайных чисел (для детерминированности).
     * @throw std::runtime_error Если матрица не положительно определена (ошибка Холецкого).
     */
    MonteCarloEngine(const Eigen::MatrixXd& covMatrix, double riskFreeRate = 0.0, uint32_t seed = 42);

    /**
     * @brief Запускает параллельную генерацию путей.
     * 
     * Разбивает задачу на батчи и исполняет их на всех доступных ядрах CPU.
     */
    std::vector<Scenario> simulate(int nPaths, int horizonDays) override;

private:

    /**
     * @brief Внутренний метод для генерации части путей (исполняется в потоке).
     * @param batchSeed Уникальный seed для конкретного потока.
     */
    std::vector<Scenario> simulateBatch(int nPaths, int horizonDays, uint32_t batchSeed);

    Eigen::MatrixXd covMatrix_;
    Eigen::MatrixXd choleskyL_; ///< Нижнетреугольная матрица (L * L^T = Sigma).
    double riskFreeRate_;
    size_t numAssets_;
    uint32_t seed_;
};

}