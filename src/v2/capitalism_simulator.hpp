#pragma once

#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <set>

namespace capitalism {
struct Person {
  double capital{0}, skill{10};
};

class Simulator {
public:
  Simulator() = delete;
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  Simulator(int people, double average_capital, double average_skill);
  void run(int duration, int sampling);

private:
  std::vector<Person> population;
  std::ofstream outfile;
  std::mt19937 random_number_generator{std::chrono::system_clock::now().time_since_epoch().count()};

  void run_step();
  double random_number_between(double from, double to);
  int random_person(std::set<int> & sample);
  void print_sample(int t);
};
} // namespace capitalism
