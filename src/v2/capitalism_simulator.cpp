#include <algorithm>
#include <iostream>
#include <numeric>
#include <sys/stat.h>

#include <capitalism_simulator.hpp>

namespace capitalism {

Simulator::Simulator(const Config &config) {
  number_of_subsets = config.subsets.size();
  dirname = config.output_dir_name;
  population.reserve(config.population);

  for (const auto &subset : config.subsets) {
    for (int i = 0; i < subset.size; i++) {
      population.push_back({subset.group, 1.0, subset.skill});
    }
  }
}

void Simulator::run(const Config &config) {
  // create output dir
  if (mkdir(dirname.c_str(), 0755) != 0) {
    std::runtime_error("Problem while creating the directory.");
  }

  top_10_file.open(dirname + "/top_10_file.dat");
  top_20_file.open(dirname + "/top_20_file.dat");

  print(0);

  for (int t = 1; t <= config.duration; ++t) {
    run_step();
    if (t % config.sampling == 0) {
      std::cout << 100. * double(t) / config.duration << " %\n";
      print(t);
    }
  }
}

void Simulator::print(int t) { print_percentages(t); }

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

void Simulator::print_percentages(int t) {
  std::vector<Person> ranked_pop{population};
  sort(ranked_pop.begin(), ranked_pop.end());

  // compute and print the total capital of the top 10% and 20%
  size_t ten_percent = ranked_pop.size() / 10;
  double total_capital = 0.;
  std::vector<int> beteilung(number_of_subsets, 0);
  for (auto it = ranked_pop.cbegin() + 9 * ten_percent; it != ranked_pop.cend();
       it++) {
    total_capital += it->capital;
    beteilung[it->group]++;
  }
  top_10_file << t << '\t' << total_capital / ranked_pop.size();
  for (int i = 0; i < number_of_subsets; ++i) {
    top_10_file << '\t' << double(beteilung[i]) / ranked_pop.size();
  }
  top_10_file << '\n';

  for (auto it = ranked_pop.cbegin() + 8 * ten_percent;
       it != ranked_pop.cend() - ten_percent; it++) {
    total_capital += it->capital;
    beteilung[it->group]++;
  }
  top_20_file << t << '\t' << total_capital / ranked_pop.size();
  for (int i = 0; i < number_of_subsets; ++i) {
    top_20_file << '\t' << double(beteilung[i]) / ranked_pop.size();
  }
  top_20_file << '\n';
}

} // namespace capitalism
