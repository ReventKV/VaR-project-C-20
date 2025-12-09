#include <gtest/gtest.h>
#include "var/MonteCarloVaRCalculator.hpp"
#include "var/Instrument.hpp"
#include "var/Portfolio.hpp"

using namespace var;

TEST(MonteCarloTest, DeterminismCheck) {
    TimeSeries ts;
    ts.add({"1", 100.0}); ts.add({"2", 101.0}); ts.add({"3", 102.0});
    std::map<std::string, TimeSeries> data;
    data["AAPL"] = ts;

    Portfolio pf;
    pf.addPosition(Position(std::make_shared<Instrument>("AAPL", 100.0), 10.0));

    uint32_t seed = 999;
    MonteCarloVaRCalculator mc1(1000, data, seed);
    MonteCarloVaRCalculator mc2(1000, data, seed);

    double var1 = mc1.calculateVaR(pf, 0.95, 1);
    double var2 = mc2.calculateVaR(pf, 0.95, 1);

    EXPECT_DOUBLE_EQ(var1, var2);

    MonteCarloVaRCalculator mc3(1000, data, seed + 1);
    double var3 = mc3.calculateVaR(pf, 0.95, 1);

    EXPECT_NE(var1, var3);
}