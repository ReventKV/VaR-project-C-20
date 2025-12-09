#include <gtest/gtest.h>
#include "var/RiskModel.hpp"
#include "var/TimeSeries.hpp"
#include "var/ReturnCalculator.hpp"
#include <cmath>
using namespace var;
TEST(RiskModelTest, CalculatesCorrectCovariance) {

    TimeSeries ts1;
    ts1.add({"2023-01-01", 100.0});
    ts1.add({"2023-01-02", 101.0}); 
    ts1.add({"2023-01-03", 104.0});

    TimeSeries ts2;
    ts2.add({"2023-01-01", 50.0});
    ts2.add({"2023-01-02", 50.5}); 
    ts2.add({"2023-01-03", 52.0}); 

    std::vector<TimeSeries> portfolio = {ts1, ts2};
    
    RiskModel model;
    Eigen::MatrixXd cov = model.computeCovarianceMatrix(portfolio);


    ASSERT_EQ(cov.rows(), 2);
    ASSERT_EQ(cov.cols(), 2);


    EXPECT_GT(cov(0,0), 0.0);
    EXPECT_GT(cov(1,1), 0.0);
    

    EXPECT_GT(cov(0,1), 0.0); 
}

TEST(ReturnCalculatorTest, LogReturnsLogic) {
    TimeSeries ts;
    ts.add({"1", 100.0});
    ts.add({"2", 200.0}); 

    auto rets = ReturnCalculator::computeLogReturns(ts);
    ASSERT_EQ(rets.size(), 1);
    EXPECT_NEAR(rets[0], 0.693147, 1e-5);
}

TEST(RiskModelTest, HandlesMismatchedDates) {

    TimeSeries tsA;
    tsA.add({"2023-01-01", 100.0});
    tsA.add({"2023-01-02", 101.0});
    tsA.add({"2023-01-03", 102.0});
    tsA.add({"2023-01-04", 103.0});


    TimeSeries tsB;
    tsB.add({"2023-01-01", 50.0});
    tsB.add({"2023-01-02", 50.5});

    tsB.add({"2023-01-04", 51.5}); 

    std::vector<TimeSeries> portfolio = {tsA, tsB};
    RiskModel model;


    Eigen::MatrixXd cov;
    ASSERT_NO_THROW({
        cov = model.computeCovarianceMatrix(portfolio);
    });

    ASSERT_EQ(cov.rows(), 2);
    ASSERT_EQ(cov.cols(), 2);
    
}