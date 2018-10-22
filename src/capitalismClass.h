/*
 * A group of individuals, initially all with the same starting capital,
 * exchange money among themselves. Each turn, they organize in couples;
 * each couple does a simple, random money transfer from an individual to the other.
 * The new money distribution is printed.
 * This process is iterated simulationLenght times.
 */

#include <fstream>
#include <random>
#include <chrono>
#include <set>

class capitalismSimulator
{
public:
  capitalismSimulator(size_t const& numberOfIndividuals,
                      long const& simulationLenght,
                      long const& startingCapitalPerIndividual,
                      std::string outputFileName)
    : simulationLenght{simulationLenght},
      simulationTime{0L}
  {
    outputFile.open(outputFileName);
    populationCapital.assign(numberOfIndividuals, startingCapitalPerIndividual);
    randomNumberGenerator.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }
  void simulate();

private:
  long simulationLenght;
  long simulationTime;
  std::vector<long> populationCapital;
  std::ofstream outputFile;
  std::mt19937 randomNumberGenerator;

  void simulationIteration();
  void printCurrentState();
  long randomNumberBetweenValues(long const&, long const&);
  size_t pickARandomPersonAndRemoveItFromTheSet(std::set<size_t> & peopleWhoDidNotExchangeYet);
};
