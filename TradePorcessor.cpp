#include "TradeProcessor.h"
void Processor::Process(std::istream& stream)
    {

        std::vector<std::string> items;
        std::string item;
        while (std::getline(stream, item))
        {
            items.push_back(item);
        }

        std::vector<TR> objects;
        int lineCount = 1;
        for (const auto& item : items)
        {
            std::vector<std::string> fields;

            std::string t;
            std::istringstream tStream(item);
            while (std::getline(tStream, t, ','))
            {
                fields.push_back(t);
            }
            if (fields.size() != 3)
            {
                std::cout << "WARN: Line " << lineCount << " malformed. Only " << fields.size() << " field(s) found." << std::endl;
                continue;
            }
            if (fields[0].length() != 6)
            {
                std::cout << "WARN: Trade currencies on line " << lineCount << " malformed: '" << fields[0] << "'" << std::endl;
                continue;
            }
            int tam;
            if (!intGetFromString(fields[1], tam))
            {
                std::cout << "WARN: Trade amount on line " << lineCount << " not a valid integer: '" << fields[1] << "'" << std::endl;
            }
            double tp;
            if (!toDouble(fields[2], tp))
            {
                std::cout << "WARN: Trade price on line " << lineCount << " not a valid decimal: '" << fields[2] << "'" << std::endl;
            }
            std::string scc = fields[0].substr(0, 3);
            std::string dcc = fields[0].substr(3, 3);
            // calculate values
            TR  obj;
            obj.SC = scc;
            obj.DC = dcc;
            obj.Lots = tam / LotSize;
            obj.Price = tp;
            objects.push_back(obj);
            lineCount++;
        }

        std::ostringstream xStream;
        xStream << "<TradeRecords>" << std::endl;
        for (const auto& record : objects) {
            xStream << "\t<TradeRecord>" << std::endl;
            xStream << "\t\t<SourceCurrency>" << record.SC << "</SourceCurrency>" << std::endl;
            xStream << "\t\t<DestinationCurrency>" << record.DC << "</DestinationCurrency>" << std::endl;
            xStream << "\t\t<Lots>" << record.Lots << "</Lots>" << std::endl;
            xStream << "\t\t<Price>" << record.Price << "</Price>" << std::endl;
            xStream << "\t</TradeRecord>" << std::endl;
        }
        xStream << "</TradeRecords>";
        std::string xData = xStream.str();
        std::ofstream outFile("output.xml"); // Output XML file
        outFile << xData;
        std::cout << "INFO: " << objects.size() << " trades processed" << std::endl;
    }
