#include <capitalism_simulator.hpp>

namespace capitalism {

Simulator::Simulator(int people, double average_capital, double average_skill) {
  population.reserve(people);

  for (int i = 0; i < people; i++) {
    population.push_back({average_capital, average_skill});
  }

  outfile.open("results.dat");
}

void Simulator::run(int duration, int sampling) {
  for (int t = 0; t < duration; ++t) {
    run_step();
    if (t % sampling == 0) {
      print_sample(t);
    }
  }
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

    double amount = random_number_between(-population[first].capital,
                                          population[secnd].capital);
    population[first].capital += amount;
    population[secnd].capital -= amount;
  }
}

double Simulator::random_number_between(double from, double to) {
  return std::uniform_real_distribution<double>(from,
                                                to)(random_number_generator);
}

int Simulator::random_person(std::set<int> &sample) {
  int random = random_number_between(0, sample.size() - 1);
  auto chosen = sample.begin();
  std::advance(chosen, random);
  return *chosen;
}

void Simulator::print_sample(int t) {
  outfile << t;
  for (const auto &i : population) {
    outfile << "\t" << i.capital;
  }
  outfile << "\n";
}

} // namespace capitalism
