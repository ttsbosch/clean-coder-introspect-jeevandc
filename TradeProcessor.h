#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct TR
{
    std::string SC;//SourceCurrency
    std::string DC;//DestibationCurrency
    float Lots;
    double Price;
};

class Processor
{
public:
    void Process(std::istream& stream);
    
private:
    static const float LotSize;
    std::vector<std::string> SplitString(const std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool intGetFromString(const std::string& str, int& value)
    {
        try
        {
            value = std::stoi(str);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    bool toDouble(const std::string& str, double& value)
    {
        try
        {
            value = std::stod(str);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
};
const float Processor::LotSize = 100000;

// int main()
// {
//     TradeProcessor processor;
//     std::ifstream file("trades.txt");
//     if (file.is_open())
//     {
//         processor.ProcessTrades(file);
//         file.close();
//     }
//     else
//     {
//         std::cout << "Failed to open file" << std::endl;
//     }
//     return 0;
// }

