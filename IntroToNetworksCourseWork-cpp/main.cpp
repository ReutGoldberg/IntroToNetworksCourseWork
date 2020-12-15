#include <iostream>
#include "Simulator.h"

using std::stod;

int main(int argc, char* argv[]) {
    vector<double> probabilities;
    for(int i=5; i < argc ; i++)
        probabilities.push_back(stod(argv[i]));
    Simulator* sim= Simulator::getInstance(int(stod(argv[1])), int(stod(argv[2])), stod(argv[3]), stod(argv[4]), probabilities);
    sim->run();
    sim->print_results();
    return 0;
}




