
#include "var/VaREngine.hpp"
#include "var/DataLoader.hpp"
#include "var/Instrument.hpp"
#include "var/ParametricVaRCalculator.hpp"
#include "var/MonteCarloVaRCalculator.hpp"
#include "var/HistoricalVaRCalculator.hpp"
#include <iostream>
#include <iomanip>
namespace var {
void VaREngine::addAsset(const std::string& ticker, const std::string& csvPath, double quantity, double currentPrice) {
    std::cout << "[Info] Loading data for " << ticker << " from " << csvPath << "...\n";
    

    DataLoader loader(csvPath);
    TimeSeries ts = loader.load();
    
    if (ts.size() == 0) {
        std::cerr << "[Warning] No data loaded for " << ticker << "!\n";
    }
    

    marketData_[ticker] = ts;


    auto instrument = std::make_shared<Instrument>(ticker, currentPrice);
    portfolio_.addPosition(Position(instrument, quantity));
}

void VaREngine::runReport(double confidence, int horizonDays, int mcPaths) {
    double totalValue = portfolio_.getTotalValue();
    std::cout << "\n=== Portfolio Risk Report ===\n";
    std::cout << "Positions: " << portfolio_.getPositions().size() << "\n";
    std::cout << "Total Value: " << std::fixed << std::setprecision(2) << totalValue << " USD\n";
    std::cout << "Confidence Level: " << confidence * 100 << "%\n";
    std::cout << "Time Horizon: " << horizonDays << " day(s)\n";
    std::cout << "-----------------------------\n";

    try {

        ParametricVaRCalculator paramCalc(marketData_);
        double paramVaR = paramCalc.calculateVaR(portfolio_, confidence, horizonDays);
        
        std::cout << "Parametric VaR:  " << paramVaR << " USD (" 
                  << (paramVaR / totalValue * 100.0) << "%)\n";


        HistoricalVaRCalculator histCalc(marketData_);
        double histVaR = histCalc.calculateVaR(portfolio_, confidence, horizonDays);
        std::cout << "Historical VaR:  " << histVaR << " USD (" 
                  << (histVaR / totalValue * 100.0) << "%)\n";



        std::cout << "Running Monte Carlo (" << mcPaths << " paths)...\n";
    
        MonteCarloVaRCalculator mcCalc(mcPaths, marketData_, 12345); 
    
        double mcVaR = mcCalc.calculateVaR(portfolio_, confidence, horizonDays);

        std::cout << "Monte Carlo VaR: " << mcVaR << " USD (" 
                  << (mcVaR / totalValue * 100.0) << "%)\n";

    } catch (const std::exception& e) {
        std::cerr << "[Error] Calculation failed: " << e.what() << "\n";
    }
    std::cout << "=============================\n";
}
}