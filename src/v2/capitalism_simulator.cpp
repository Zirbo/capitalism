#include <algorithm>
#include <iostream>
#include <numeric>
#include <sys/stat.h>

#include <capitalism_simulator.hpp>

namespace capitalism {

Simulator::Simulator(const Config &config) {
  dirname = config.output_dir_name;
  population.reserve(config.population);

  for (int i = 0; i < config.population; i++) {
    double skill = 1.0;
    population.push_back({1.0, skill});
  }
  histogram_size = double(config.population);
  bin_size = histogram_size / number_of_bins;
}

void Simulator::run(const Config &config) {
  // create output dir
  if (mkdir(dirname.c_str(), 0755) != 0) {
    std::runtime_error("Problem while creating the directory.");
  }

  sample_file.open(dirname + "/full_sample.dat");
  maxes_file.open(dirname + "/maxes.dat");
  percentages_file.open(dirname + "/percentages.dat");

  print(0);

  for (int t = 1; t <= config.duration; ++t) {
    run_step();
    if (t % config.sampling == 0) {
      std::cout << 100. * double(t) / config.duration << " %\n";
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
  // rank the capitals
  std::vector<double> capitals;
  for (const auto &i : population) {
    capitals.push_back(i.capital);
  }
  sort(capitals.begin(), capitals.end());

  // print the top ten
  maxes_file << t;
  for (int i = capitals.size() - 10; i < capitals.size(); ++i) {
    maxes_file << '\t' << capitals[i];
  }
  maxes_file << '\n';

  // compute and print the total capital of the top 10% and 20%
  size_t ten_percent = capitals.size() / 10;
  double m10 = std::accumulate(capitals.begin() + 8 * ten_percent,
                               capitals.end() - ten_percent, 0.);
  double m20 =
      std::accumulate(capitals.begin() + 9 * ten_percent, capitals.end(), m10);
  m10 /= capitals.size();
  m20 /= capitals.size();

  percentages_file << t << '\t' << m10 << '\t' << m20 << '\n';
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
  std::string filename = dirname + "/histogram_file_t_";
  filename += std::to_string(t) + ".txt";
  std::ofstream file(filename);

  // output histogram
  for (int i = 0; i < number_of_bins; ++i) {
    file << bin_size * i << '\t' << double(bins[i]) / population.size() << '\n';
  }
  file << number_of_bins * bin_size << '\t' << 0 << '\n';
}

} // namespace capitalism
