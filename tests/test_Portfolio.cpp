#include <gtest/gtest.h>
#include "var/Portfolio.hpp"
#include <memory>
using namespace var;
TEST(PortfolioTest, CalculatesTotalValue) {

    auto apple = std::make_shared<Instrument>("AAPL", 150.0);
    auto google = std::make_shared<Instrument>("GOOGL", 2800.0);

    Position pos1(apple, 10.0);  // 1500
    Position pos2(google, 1.0);  // 2800


    Portfolio pf;
    pf.addPosition(pos1);
    pf.addPosition(pos2);

    // Ожидаемая сумма: 150 * 10 + 2800 * 1 = 4300
    EXPECT_DOUBLE_EQ(pf.getTotalValue(), 4300.0);
}

TEST(PortfolioTest, UpdatesValueWhenPriceChanges) {
    auto apple = std::make_shared<Instrument>("AAPL", 100.0);
    Position pos(apple, 5.0); // 500

    Portfolio pf;
    pf.addPosition(pos);

    EXPECT_DOUBLE_EQ(pf.getTotalValue(), 500.0);


    apple->setPrice(110.0);

    // 110 * 5 = 550
    EXPECT_DOUBLE_EQ(pf.getTotalValue(), 550.0);
}
