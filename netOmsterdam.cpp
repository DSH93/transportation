#include <iostream>
#include "MultiGraph.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Program Argument are Missing" << std::endl;
        exit(-1);
    }
    MultiGraph graph;
    try {
        for (int i = 1; i < argc; i++) {
            graph.readInput(argv[i]);
        }

    } catch (const char *e) {
        std::cerr << e << std::endl;

    }
    graph.start();
    return 0;

}