#include <fstream>
#include <random>
#include <chrono>
#include <set>

class capitalismSimulator
{
  long simulationLenght;
  long simulationTime;
  std::vector<long> populationCapital;
  std::ofstream outputFile;
  std::mt19937 randomNumberGenerator;

  void simulationIteration();
  void printCurrentState();
  long randomNumberBetweenValues(long const&, long const&);
  size_t pickAPerson(std::set<long> & peopleWhoDidNotExchangeYet);

public:
  capitalismSimulator(size_t const& numberOfParticles,
                           long const& simulationLenght,
                           long const& startingInitialCapital,
                           std::string outputFileName)
    : simulationLenght{simulationLenght},
      simulationTime{0L}
  {
    outputFile.open(outputFileName);
    populationCapital.assign(numberOfParticles, startingInitialCapital);
    randomNumberGenerator.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }
  void simulate();

};
