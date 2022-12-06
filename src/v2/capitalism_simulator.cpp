#include <algorithm>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

#include <capitalism_simulator.hpp>

namespace capitalism {

Simulator::Simulator(int people) {
  population.reserve(people);

  for (int i = 0; i < people; i++) {
    double skill = 1.0;
    population.push_back({1.0, skill});
  }
  histogram_size = double(people);
  bin_size = histogram_size / number_of_bins;
}

void Simulator::run(int duration, int sampling) {
  // create output dir
  std::stringstream dirname_ss;
  dirname_ss << "rundata_n_" << population.size() << "_t_" << duration << "_s_"
             << sampling << '/';
  dirname = dirname_ss.str();
  if (mkdir(dirname.c_str(), 0755) != 0) {
    std::runtime_error("Problem while creating the directory.");
  }

  sample_file.open(dirname + "full_sample.dat");
  maxes_file.open(dirname + "maxes.dat");

  print(0);

  for (int t = 1; t <= duration; ++t) {
    run_step();
    if (t % sampling == 0) {
      std::cout << 100. * double(t) / duration << " %\n";
      print(t);
    }
  }
}

void Simulator::print(int t) {
  print_sample(t);
  // print_histogram(t);
  print_maxes(t);
}

void Simulator::run_step() {
  // start with a set containing all the people IDs;
  // form couples and make an exchange until no couples are left
  std::set<int> people_yet_to_exchange;
  for (size_t i = 0; i < population.size(); ++i) {
    people_yet_to_exchange.insert(i);
  }

  while (!people_yet_to_exchange.empty()) {
    int first = random_person(people_yet_to_exchange);
    int secnd = random_person(people_yet_to_exchange);
    people_yet_to_exchange.erase(first);
    people_yet_to_exchange.erase(secnd);

    // First we randomly determine the fractional exchange.
    // The higher the skill, the bigger the chance to win AND the
    // higher the payoff.
    // If the number is negative, the first individual wins and
    // receives that % of the second individual's capital;
    // viceversa if positive.
    double amount = random_double_between(-population[first].skill,
                                          population[secnd].skill);
    if (amount < 0) {
      amount *= population[secnd].capital;
    } else {
      amount *= population[first].capital;
    }

    population[first].capital -= amount;
    population[secnd].capital += amount;
  }
}

double Simulator::random_double_between(double from, double to) {
  return std::uniform_real_distribution<double>(from,
                                                to)(random_number_generator);
}

int Simulator::random_int_between(int from, int to) {
  return std::uniform_int_distribution<int>(from, to)(random_number_generator);
}

int Simulator::random_person(std::set<int> &sample) {
  int random = random_int_between(0, sample.size());
  auto chosen = sample.begin();
  std::advance(chosen, random);
  return *chosen;
}

void Simulator::print_sample(int t) {
  sample_file << t;
  for (const auto &i : population) {
    sample_file << '\t' << i.capital;
  }
  sample_file << '\n';
}

void Simulator::print_maxes(int t) {
  std::vector<double> copy;
  for (const auto &i : population) {
    copy.push_back(i.capital);
  }
  sort(copy.begin(), copy.end());
  maxes_file << t;
  for (int i = copy.size() - 10; i < copy.size(); ++i) {
    maxes_file << '\t' << copy[i];
  }
  maxes_file << '\n';
}

void Simulator::print_histogram(int t) {
  // compute histogram
  std::vector<int> bins(number_of_bins, 0.);
  for (const auto &i : population) {
    int bin = int(i.capital / bin_size);
    if (bin > number_of_bins) {
      bin = number_of_bins - 1;
    }
    bins[bin]++;
  }

  // create output file
  std::string filename = dirname + "histogram_file_t_";
  filename += std::to_string(t) + ".txt";
  std::ofstream file(filename);

  // output histogram
  for (int i = 0; i < number_of_bins; ++i) {
    file << bin_size * i << '\t' << double(bins[i]) / population.size() << '\n';
  }
  file << number_of_bins * bin_size << '\t' << 0 << '\n';
}

} // namespace capitalism
