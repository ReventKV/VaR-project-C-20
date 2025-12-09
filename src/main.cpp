#include "var/VaREngine.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <limits>

using namespace var;


void createDummyCSV(const std::string& filename, double startPrice, double trend, double vol) {
    std::ofstream file(filename);
    file << "Date,Price\n";
    double price = startPrice;
    std::mt19937 gen(42); 
    std::normal_distribution<> d(0, 1);
    for (int i = 0; i < 250; ++i) {
        file << "2023-01-" << (i + 1) << "," << price << "\n";
        double ret = trend + vol * d(gen);
        price *= std::exp(ret);
    }
    std::cout << "[Demo] Generated file: " << filename << std::endl;
}


void runDemoMode(VaREngine& engine) {
    std::cout << "\n--- Running Demo Mode ---\n";

    createDummyCSV("aapl_demo.csv", 150.0, 0.0005, 0.015); 
    createDummyCSV("tsla_demo.csv", 700.0, 0.0010, 0.035);


    engine.addAsset("AAPL", "aapl_demo.csv", 10, 150.0);
    engine.addAsset("TSLA", "tsla_demo.csv", 2, 700.0);
}


void runUserMode(VaREngine& engine) {
    std::cout << "\n--- Custom Portfolio Mode ---\n";
    std::cout << "Tip: Ensure your CSV files are in the current folder or provide full paths.\n";
    std::cout << "Format: Date (YYYY-MM-DD), Price (123.45)\n\n";

    while (true) {
        std::string ticker, path;
        double qty, price;

        std::cout << "Enter Asset Ticker (e.g. BTC) or 'done' to finish: ";
        std::cin >> ticker;
        if (ticker == "done") break;

        std::cout << "Path to CSV file (e.g. bitcoin.csv): ";
        std::cin >> path;

        std::cout << "Quantity (e.g. 0.5): ";
        while(!(std::cin >> qty)) {
            std::cout << "Invalid number. Try again: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Current Market Price (e.g. 95000): ";
        while(!(std::cin >> price)) {
            std::cout << "Invalid number. Try again: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }


        try {
            engine.addAsset(ticker, path, qty, price);
            std::cout << "Asset " << ticker << " added successfully.\n\n";
        } catch (const std::exception& e) {
            std::cerr << "[Error] Failed to add asset: " << e.what() << "\n\n";
        }
    }
}

int main() {
    try {
        VaREngine engine;
        int choice;

        std::cout << "================================\n";
        std::cout << "   C++20 VaR Engine (CLI)       \n";
        std::cout << "================================\n";
        std::cout << "Select Mode:\n";
        std::cout << "1. Demo (Generate data)\n";
        std::cout << "2. Custom (Load your CSV files)\n";
        std::cout << "> ";
        
        if (!(std::cin >> choice)) return 0;

        if (choice == 1) {
            runDemoMode(engine);
        } else if (choice == 2) {
            runUserMode(engine);
        } else {
            std::cerr << "Invalid choice.\n";
            return 1;
        }


        double conf;
        int days;
        int paths = 10000;

        std::cout << "\nEnter Confidence Level (0.95 or 0.99): ";
        std::cin >> conf;
        std::cout << "Enter Horizon (days): ";
        std::cin >> days;


        engine.runReport(conf, days, paths);

    } catch (const std::exception& e) {
        std::cerr << "\nFatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}