// tests/test_DataLoader.cpp
#include <gtest/gtest.h>
#include "var/DataLoader.hpp"

using namespace var;
// Тест только для парсера строк (Unit test)
TEST(CsvReaderTest, ParseLineSplitsCorrectly) {
    CsvReader reader(',');
    std::string line = "2023-10-01,150.5,SomeOtherData";
    
    auto tokens = reader.parseLine(line);
    
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "2023-10-01");
    EXPECT_EQ(tokens[1], "150.5");
    EXPECT_EQ(tokens[2], "SomeOtherData");
}

// Тест для TimeSeries (проверка логики хранения)
TEST(TimeSeriesTest, AddAndRetrieve) {
    TimeSeries ts;
    ts.add({"2023-01-01", 100.0});
    ts.add({"2023-01-02", 101.5});

    auto prices = ts.getPriceSeries();
    ASSERT_EQ(prices.size(), 2);
    EXPECT_DOUBLE_EQ(prices[0], 100.0);
    EXPECT_DOUBLE_EQ(prices[1], 101.5);
}