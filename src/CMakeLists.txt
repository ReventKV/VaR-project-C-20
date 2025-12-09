# --- Библиотека ядра (Core) ---

add_library(var_core 
    DataLoader.cpp
    ReturnCalculator.cpp
    RiskModel.cpp
    Portfolio.cpp
    ParametricVaRCalculator.cpp
    MonteCarloEngine.cpp
    MonteCarloVaRCalculator.cpp
    MCStatistics.cpp
    VaREngine.cpp
    HistoricalVaRCalculator.cpp
)
target_include_directories(var_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)


find_package(Threads REQUIRED)


target_link_libraries(var_core PUBLIC 
    Eigen3::Eigen
    Threads::Threads
)

add_executable(var_app main.cpp)
target_link_libraries(var_app PRIVATE var_core)