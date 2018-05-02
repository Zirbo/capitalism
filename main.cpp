#include <iostream>
#include <fstream>
#include <fstream>
#include <random>
#include <chrono>
#include <set>

void simulate (long, long, long );
void iteration(std::vector<long>&);
long rand_long(long, long);

std::mt19937 RNG (std::chrono::system_clock::now().time_since_epoch().count());

int main( int argc, char **argv )
{
    if( argc != 4 )
    {
        std::cout << "Insert the number of people, the simulation lenght in time steps, "
                  << "and the initial capital (all positive integers).\n"
                  << "Check the readme for more information." << std::endl;
        return 1;
    }
    long nPeople = std::stol(argv[1]);
    if (nPeople%2 != 0)
    {
        std::cout << "The number of people must be even!\n" << std::endl;
        return 1;
    }
    long time    = std::stol(argv[2]);
    long capital = std::stol(argv[3]);
    std::cout << "You typed " << nPeople << ", " << time << ", " << capital << ".\n";

    simulate(nPeople, time, capital);
    return 0;
}

void simulate(long Np, long Tf, long Ci)
{
    std::ofstream OUT("output.txt");
    // fill the population vector, everyone starts with the same capital
    std::vector<long> population(Np, Ci);

    // run Tf bouts of exchanges
    for (long i = 1; i <= Tf; ++i)
    {
        std::cout << "Iteration number " << i << "\n";
        iteration(population);
        // now print the state
        OUT << i;
        for (long i = 0; i != population.size(); ++i)
           OUT << "\t" << population[i];
        OUT << std::endl;
    }
}

void iteration(std::vector<long>& pop)
{
    // initialize a random number generator
    // define an vector to determine if the people have already had their interaction in this turn

    std::set<long> exchanged;
    for (long i = 0; i != pop.size(); ++i)
    {
        exchanged.insert(i);
    }

    std::cout << "Start!\n";
    // start!
    while (!exchanged.empty())
    {
        std::set<long>::iterator it = exchanged.begin();
        // pick a first person
        long randomSelect = rand_long(0,exchanged.size()-1);
        std::advance(it,randomSelect);
        long firstPerson = *it;
        exchanged.erase(it);
        // pick a second person
        randomSelect = rand_long(0,exchanged.size()-1);
        it = exchanged.begin();
        std::advance(it,randomSelect);
        long secondPerson = *it;
        exchanged.erase(it);

        // now do the exchange
        long amount = rand_long(-pop[firstPerson], pop[secondPerson]);
        pop[firstPerson]  += amount;
        pop[secondPerson] -= amount;
    }
}


long rand_long(long from, long to)
{
    return std::uniform_int_distribution<long>(from, to)(RNG);
}

