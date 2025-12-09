#include "var/SimulationEngine.hpp"
#include <cmath>
#include <iostream>
#include <future> 
#include <thread>

namespace var {

MonteCarloEngine::MonteCarloEngine(const Eigen::MatrixXd& covMatrix, double riskFreeRate, uint32_t seed) 
    : covMatrix_(covMatrix), riskFreeRate_(riskFreeRate), seed_(seed) {
    
    numAssets_ = covMatrix.rows();

    Eigen::LLT<Eigen::MatrixXd> lltOfA(covMatrix_);
    if (lltOfA.info() == Eigen::NumericalIssue) {
        throw std::runtime_error("Covariance matrix is not positive definite!");
    }
    choleskyL_ = lltOfA.matrixL();
}


std::vector<Scenario> MonteCarloEngine::simulateBatch(int nPaths, int horizonDays, uint32_t batchSeed) {
    std::vector<Scenario> batchResults;
    batchResults.reserve(nPaths);


    std::mt19937 gen(batchSeed);
    std::normal_distribution<> d(0, 1);

    double dt = double(horizonDays);

    for (int i = 0; i < nPaths; ++i) {
        Eigen::VectorXd z(numAssets_);
        for (size_t j = 0; j < numAssets_; ++j) {
            z(j) = d(gen);
        }

        Eigen::VectorXd correlatedStochastic = (choleskyL_ * z) * std::sqrt(dt);

        Eigen::VectorXd drift(numAssets_);
        for (size_t j = 0; j < numAssets_; ++j) {
            double sigmaSq = covMatrix_(j, j); 
            drift(j) = (riskFreeRate_ - 0.5 * sigmaSq) * dt;
        }

        Eigen::VectorXd totalLogReturn = drift + correlatedStochastic;

        Scenario scen(numAssets_);
        for (size_t j = 0; j < numAssets_; ++j) {
            scen[j] = totalLogReturn(j);
        }
        batchResults.push_back(scen);
    }
    return batchResults;
}

std::vector<Scenario> MonteCarloEngine::simulate(int nPaths, int horizonDays) {

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;


    if (nPaths < 1000) {
        return simulateBatch(nPaths, horizonDays, seed_);
    }

    std::vector<std::future<std::vector<Scenario>>> futures;
    int pathsPerThread = nPaths / numThreads;
    int remainingPaths = nPaths % numThreads;

    for (unsigned int i = 0; i < numThreads; ++i) {
        int count = pathsPerThread + (i < (unsigned)remainingPaths ? 1 : 0);
        

        uint32_t batchSeed = seed_ + i;

        futures.push_back(std::async(std::launch::async, 
            &MonteCarloEngine::simulateBatch, this, count, horizonDays, batchSeed));
    }


    std::vector<Scenario> finalResults;
    finalResults.reserve(nPaths);

    for (auto& f : futures) {
        auto batch = f.get();

        finalResults.insert(finalResults.end(), 
                            std::make_move_iterator(batch.begin()), 
                            std::make_move_iterator(batch.end()));
    }

    return finalResults;
}

}