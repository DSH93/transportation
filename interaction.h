//
// Created by Dor Shukrun on 05/03/2024.
//

#ifndef HW02ADV_INTERACTION_H
#define HW02ADV_INTERACTION_H


#include <string>
#include <vector>

class interaction {


public:
    interaction();

    static std::vector<std::string>tokenizeInput(const std::string &input);

    static std::vector<std::string> userInteraction();

    static void countingSpaces(const std::string& vehicle);

    static void notExistExceptions(const std::string &station);

    static void fileNotExceptions(const std::string &file);

    static void exception(const std::string &msg);

    static void printUniExpress(int time);

    static void
    printMultiExpress(int time, const std::string &source, const std::string &dest, std::vector<std::string> vector);

    static void printMultiGraph(const std::vector<std::vector<std::string>> &vector, const std::string &fileName);

    static void prepareBeforePrinting(const std::vector<std::pair<std::string, std::string>> &stations);


    static void
    printViaExpress(const std::string &src, const std::string &via, const std::string &dest,
                    int until,
                    int after);
};


#endif //HW02ADV_INTERACTION_H
