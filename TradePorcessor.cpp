#include "TradeProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <optional>

// Assumed definitions for TR struct and utility functions.
struct TR {
    std::string sourceCurrency;
    std::string destinationCurrency;
    double lots;
    double price;
};

const int LOT_SIZE = 1000; // Assuming a constant LotSize for the calculations

bool intGetFromString(const std::string &str, int &val) {
    try {
        val = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool toDouble(const std::string &str, double &val) {
    try {
        val = std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> readLines(std::istream &stream) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::optional<TR> parseLine(const std::string &line, int lineNumber) {
    auto fields = split(line, ',');
    if (fields.size() != 3) {
        std::cout << "WARN: Line " << lineNumber << " malformed. Only " << fields.size() << " field(s) found." << std::endl;
        return std::nullopt;
    }

    const std::string &currencyPair = fields[0];
    if (currencyPair.length() != 6) {
        std::cout << "WARN: Trade currencies on line " << lineNumber << " malformed: '" << currencyPair << "'" << std::endl;
        return std::nullopt;
    }

    int tradeAmount;
    if (!intGetFromString(fields[1], tradeAmount)) {
        std::cout << "WARN: Trade amount on line " << lineNumber << " not a valid integer: '" << fields[1] << "'" << std::endl;
        return std::nullopt;
    }

    double tradePrice;
    if (!toDouble(fields[2], tradePrice)) {
        std::cout << "WARN: Trade price on line " << lineNumber << " not a valid decimal: '" << fields[2] << "'" << std::endl;
        return std::nullopt;
    }

    TR tradeRecord;
    tradeRecord.sourceCurrency = currencyPair.substr(0, 3);
    tradeRecord.destinationCurrency = currencyPair.substr(3, 3);
    tradeRecord.lots = static_cast<double>(tradeAmount) / LOT_SIZE;
    tradeRecord.price = tradePrice;

    return tradeRecord;
}

void writeXmlFile(const std::vector<TR> &trades, const std::string &filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Unable to open output file." << std::endl;
        return;
    }

    outFile << "<TradeRecords>" << std::endl;
    for (const auto &record : trades) {
        outFile << "\t<TradeRecord>" << std::endl;
        outFile << "\t\t<SourceCurrency>" << record.sourceCurrency << "</SourceCurrency>" << std::endl;
        outFile << "\t\t<DestinationCurrency>" << record.destinationCurrency << "</DestinationCurrency>" << std::endl;
        outFile << "\t\t<Lots>" << record.lots << "</Lots>" << std::endl;
        outFile << "\t\t<Price>" << record.price << "</Price>" << std::endl;
        outFile << "\t</TradeRecord>" << std::endl;
    }
    outFile << "</TradeRecords>";
}

void Processor::Process(std::istream &stream) {
    auto lines = readLines(stream);
    std::vector<TR> trades;
    int lineNumber = 1;

    for (const auto &line : lines) {
        auto trade = parseLine(line, lineNumber);
        if (trade) {
            trades.push_back(*trade);
        }
        lineNumber++;
    }

    writeXmlFile(trades, "output.xml");
    std::cout << "INFO: " << trades.size() << " trades processed" << std::endl;
}
