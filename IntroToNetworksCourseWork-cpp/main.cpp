#include <iostream>
#include "Event.h"
#include <cstdlib>

using std::stod;

int main(int argc, char* argv[]) {
    vector<double> probabilities;
    for(int i=1;i<4;i++)
        probabilities.push_back(stod(argv[i]));
    Simulator* sim= Simulator::getInstance((int)argv[1], (int)argv[2], stod(argv[3]), stod(argv[4]), probabilities);
    sim->run();
    sim->print_results();
    return 0;
}




