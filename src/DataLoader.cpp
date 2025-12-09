#include "var/DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>


namespace var {
    
std::vector<std::string> CsvReader::parseLine(const std::string& line) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    

    while (std::getline(tokenStream, token, delimiter_)) {
        tokens.push_back(token);
    }
    return tokens;
}


TimeSeries DataLoader::load() {
    std::ifstream file(filePath_);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath_);
    }

    TimeSeries ts;
    CsvReader reader(','); 

    std::string line;

    
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        auto tokens = reader.parseLine(line);
        
        if (tokens.size() < 2) continue;

        try {
            
            double price = std::stod(tokens[1]); 
            ts.add({tokens[0], price});
        } catch (...) {
            continue;
        }
    }

    return ts;
}
}