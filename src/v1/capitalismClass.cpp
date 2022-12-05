#include "capitalismClass.h"
#include <iostream>

void capitalismSimulator::simulate()
{
    for (simulationTime = 1; simulationTime <= simulationLenght; ++simulationTime)
    {
        bool isCompletionPercentageMultipleOfFive = (20*simulationTime%simulationLenght) == 0;
        if (isCompletionPercentageMultipleOfFive)
          std::cout << "Time: " << simulationTime << "/" << simulationLenght <<"\n";
        simulationIteration();
        printCurrentState();
    }
}

void capitalismSimulator::simulationIteration()
{
  // start with a set containing all the people IDs;
  // form couples and make an exchange until no couples are left
  std::set<size_t> peopleWhoDidNotExchangeYet;
  for (size_t i = 0; i < populationCapital.size(); ++i)
      peopleWhoDidNotExchangeYet.insert(i);

  while (!peopleWhoDidNotExchangeYet.empty())
  {
      size_t firstPerson = pickARandomPersonAndRemoveItFromTheSet(peopleWhoDidNotExchangeYet);
      size_t secondPerson = pickARandomPersonAndRemoveItFromTheSet(peopleWhoDidNotExchangeYet);

      long amount = randomNumberBetweenValues(-populationCapital[firstPerson], populationCapital[secondPerson]);
      populationCapital[firstPerson]  += amount;
      populationCapital[secondPerson] -= amount;
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

size_t capitalismSimulator::pickARandomPersonAndRemoveItFromTheSet(std::set<size_t> & peopleWhoDidNotExchangeYet)
{
  size_t randomElementInTheSet= randomNumberBetweenValues(0,peopleWhoDidNotExchangeYet.size()-1);
  auto chosenPerson = peopleWhoDidNotExchangeYet.begin();
  std::advance(chosenPerson, randomElementInTheSet);
  size_t chosenPersonReturnValue = *chosenPerson;
  peopleWhoDidNotExchangeYet.erase(chosenPerson);
  return chosenPersonReturnValue;
}
