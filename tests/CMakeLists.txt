# tests/CMakeLists.txt


add_executable(var_tests 
    test_main.cpp
    test_DataLoader.cpp
    test_RiskModel.cpp
    test_Portfolio.cpp
    test_VaR.cpp
    test_MonteCarlo.cpp 
)


target_link_libraries(var_tests PRIVATE 
    var_core 
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(var_tests)