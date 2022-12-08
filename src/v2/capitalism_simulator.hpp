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

  Simulator(int people);
  void run(int duration, int sampling);

private:
  std::vector<Person> population;
  std::mt19937 random_number_generator{std::chrono::system_clock::now().time_since_epoch().count()};
  std::ofstream sample_file;
  std::ofstream maxes_file;
  std::ofstream percentages_file;
  int number_of_bins{100};
  double bin_size, histogram_size;
  std::string dirname;

  void run_step();
  double random_double_between(double from, double to);
  int random_int_between(int from, int to);
  int random_person(std::set<int> & sample);

  void print(int t);
  void print_sample(int t);
  void print_histogram(int t);
  void print_maxes(int t);
};
} // namespace capitalism
