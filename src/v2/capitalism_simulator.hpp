#pragma once

#include <chrono>
#include <fstream>
#include <random>
#include <set>
#include <vector>

#include <config_parser.hpp>

namespace capitalism {
struct Person {
  int group;
  double capital{0}, skill{10};

  bool operator<(const Person &other) { return this->capital < other.capital; }
};

class Simulator {
public:
  Simulator() = delete;
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  Simulator(const Config &config);
  void run(const Config &config);

private:
  std::vector<Person> population;
  int number_of_subsets{0};
  std::mt19937 random_number_generator{
      std::chrono::system_clock::now().time_since_epoch().count()};
  std::string dirname;

  std::ofstream top_10_file;
  std::ofstream top_20_file;

  void run_step();
  double random_double_between(double from, double to);
  int random_int_between(int from, int to);
  int random_person(std::set<int> &sample);

  void print(int t);
  void print_percentages(int t);
};
} // namespace capitalism
