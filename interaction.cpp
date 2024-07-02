//
// Created by Dor Shukrun on 05/03/2024.
//

#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

#include "interaction.h"

interaction::interaction() = default;

std::vector<std::string> interaction::tokenizeInput(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        std::string prefix_input = token.substr(0, 2);
        if (prefix_input == "IC" || prefix_input == "CS") token = token.substr(2);
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> interaction::userInteraction() {
    std::string line;
    std::cin.clear();
    std::getline(std::cin, line);
    auto tokens = tokenizeInput(line);
    std::cin.clear();
    line.clear();
    return tokens;
}

void interaction::countingSpaces(const std::string &vehicle) {
    std::cout << vehicle << ":";
    unsigned int spaces = 13 - vehicle.size();
    for (int i = 0; i < spaces; ++i) {
        std::cout << " ";
    }
}

std::vector<std::string> splitStringBySpace(const std::string &str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}


void interaction::printMultiGraph(const std::vector<std::vector<std::string>> &vector, const std::string &fileName) {

    std::ofstream outFile;
    outFile.open(fileName);

    outFile << " This graph shows a city's transportation network for buses, trams, sprinters,"
               " and rails.\n Each line represents a route from a starting station"
               " ('from' in square brackets)\n to a destination station (in parentheses),"
               " with the travel time in between.\n Routes are sorted by transport type,"
               " highlighting the network's connectivity and travel options." << std::endl;

    int counter;
    for (auto &i: vector) {
        counter = 0;
        if (i.size() == 1) continue;
        if (i[0] == "bus" || i[0] == "rail" || i[0] == "tram" || i[0] == "sprinter") {
            std::string vehicle = i[0];
            vehicle[0] = std::toupper(vehicle[0]);
            outFile << "\n. .  .  .  .  .  .  .  . " << (vehicle) << " .  .  .  .  .  .  .  .  .  .  .  .  . "
                    << std::endl;
            continue;
        }

        for (const auto &j: i) {
            if (counter == 0) outFile << " from: [ " << j << " ] ";
            else {
                auto tokens = splitStringBySpace(j);
                outFile << " --- " << tokens[0] << " ---> (" << tokens[1] << ")";
            }
            counter++;
        }
        outFile << std::endl;
    }


}

void interaction::prepareBeforePrinting(const std::vector<std::pair<std::string, std::string>> &stations) {
    for (const auto &pair: stations) {
        std::string type = pair.first;
        std::string st = pair.second;
        if (type == "stad") type = "";
        std::cout << type << st << "    ";
    }

    if (stations.empty()) std::cout << "no outbound travel";
    std::cout << std::endl;
}

void interaction::printUniExpress(int time) {
    if (time != -1) std::cout << time << std::endl;
    else std::cout << "routh unavailable" << std::endl;
}

void interaction::printMultiExpress(int time, const std::string &source, const std::string &dest,
                                    std::vector<std::string> vector) {

    vector.pop_back();
    if (time != -1) {
        std::cout << "Total time: " << time << " minutes" << std::endl;
        std::cout << "[ " << source << " ] ";
        for (int i = vector.size() - 2; i >= 0; i--) {
            std::cout << " ---> [ " << vector[i] << " ]";
        }
        std::cout << " ---> [ " << dest << " ]\n";

    } else std::cout << "routh unavailable" << std::endl;
}

void interaction::printViaExpress(const std::string &src, const std::string &via, const std::string &dest, int until,
                                  int after) {
    if (until != -1 && after != -1) {
        std::cout << "Traveling from '" << src << "' to '" << dest << "' via '" << via << "' takes a total of "
                  << until + after << " minutes." << std::endl;
        std::cout << src << " --- " << until << " ---> " << via << " --- " << after << "---> " << dest << std::endl;
    } else std::cout << "routh unavailable" << std::endl;

}

void interaction::notExistExceptions(const std::string &station) {
    std::cerr << "ERROR: The specified '" << station << "' station does not exist." << std::endl;
}

void interaction::fileNotExceptions(const std::string &file) {
    std::cerr << "ERROR: opening the specified file '" << file << "'" << std::endl;

}

void interaction::exception(const std::string &msg) {
    std::cerr << msg << std::endl;
}
