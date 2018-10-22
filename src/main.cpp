#include <iostream>
#include "capitalismClass.h"

int main( int argc, char **argv )
{
    if( argc != 4 )
    {
        std::cout << "Insert the number of people, the simulation lenght in time steps, "
                  << "and the initial capital (all positive integers).\n"
                  << "Check the readme for more information." << std::endl;
        return 1;
    }
    long numberOfPeopleInTheSample = std::stol(argv[1]);
    if (numberOfPeopleInTheSample%2 != 0)
    {
        std::cout << "The number of people must be even!\n" << std::endl;
        return 1;
    }
    long simulationLenght = std::stol(argv[2]);
    long startingCapital = std::stol(argv[3]);
    std::cout << "You typed " << numberOfPeopleInTheSample << ", "
              << simulationLenght << ", "
              << startingCapital << ".\n";

    capitalismSimulator simulation(numberOfPeopleInTheSample, simulationLenght,
                                   startingCapital, "output.txt");
    simulation.simulate();
    return 0;
}
