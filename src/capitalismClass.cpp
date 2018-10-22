#include "capitalismClass.h"
#include <iostream>

void capitalismSimulator::simulate()
{
    for (simulationTime = 1; simulationTime <= simulationLenght; ++simulationTime)
    {
        bool isPercentageMultipleOfFive = (20*simulationTime%simulationLenght) == 0;
        if (isPercentageMultipleOfFive)
          std::cout << "Time: " << simulationTime << "/" << simulationLenght <<"\n";
        simulationIteration();
        printCurrentState();
    }
}

void capitalismSimulator::printCurrentState()
{
  outputFile << simulationTime;
  for (size_t i = 0; i != populationCapital.size(); ++i)
     outputFile << "\t" << populationCapital[i];
  outputFile << std::endl;
}

long capitalismSimulator::randomNumberBetweenValues(long const& from, long const& to)
{
    return std::uniform_int_distribution<long>(from, to)(randomNumberGenerator);
}

size_t capitalismSimulator::pickAPersonAndRemoveItFromTheSet(std::set<long> & peopleWhoDidNotExchangeYet)
{
  long randomElementInTheSet= randomNumberBetweenValues(0,peopleWhoDidNotExchangeYet.size()-1);
  auto chosenPerson = peopleWhoDidNotExchangeYet.begin();
  std::advance(chosenPerson, randomElementInTheSet);
  size_t chosenPersonReturnValue = *chosenPerson;
  peopleWhoDidNotExchangeYet.erase(chosenPerson);
  return chosenPersonReturnValue;
}

void capitalismSimulator::simulationIteration()
{
  // start with a set with all the people;
  // form couples and exchange until no couples are left
  std::set<long> peopleWhoDidNotExchangeYet;
  for (long i = 0; i < populationCapital.size(); ++i)
      peopleWhoDidNotExchangeYet.insert(i);

  while (!peopleWhoDidNotExchangeYet.empty())
  {
      size_t firstPerson = pickAPersonAndRemoveItFromTheSet(peopleWhoDidNotExchangeYet);
      size_t secondPerson = pickAPersonAndRemoveItFromTheSet(peopleWhoDidNotExchangeYet);

      long amount = randomNumberBetweenValues(-populationCapital[firstPerson], populationCapital[secondPerson]);
      populationCapital[firstPerson]  += amount;
      populationCapital[secondPerson] -= amount;
  }
}



