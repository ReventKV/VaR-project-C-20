#include <gtest/gtest.h>
#include "var/ParametricVaRCalculator.hpp"
#include "var/Instrument.hpp"
#include "var/Portfolio.hpp"
using namespace var;
TEST(ParametricVaRTest, CalculatesRiskCorrectly) {
    TimeSeries tsA;
    tsA.add({"1", 100.0}); tsA.add({"2", 101.0}); tsA.add({"3", 102.0}); 


    std::map<std::string, TimeSeries> marketData;
    marketData["A"] = tsA;


    auto instrA = std::make_shared<Instrument>("A", 100.0);
    Portfolio pf;
    pf.addPosition(Position(instrA, 10.0));


    ParametricVaRCalculator calc(marketData);


    double var = calc.calculateVaR(pf, 0.95, 1);

    EXPECT_GT(var, 0.0);
}

TEST(ParametricVaRTest, ThrowsOnMissingData) {
    std::map<std::string, TimeSeries> marketData;
    
    auto instrB = std::make_shared<Instrument>("B", 50.0);
    Portfolio pf;
    pf.addPosition(Position(instrB, 1.0));

    ParametricVaRCalculator calc(marketData);
    
    EXPECT_THROW(calc.calculateVaR(pf, 0.95, 1), std::runtime_error);
}