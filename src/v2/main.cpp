#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include <capitalism_simulator.hpp>
#include <config_parser.hpp>

static void usage(const char *binary_name) {
  std::cerr << "\nUsage:\n" << binary_name << " -c *config*\n";
  exit(EXIT_FAILURE);
}

void do_simulation(const char *config_path) {
  capitalism::Config config(config_path);
  capitalism::Simulator sim{config};
  sim.run(config);
}

int main(int argc, char *argv[]) {
  char *config_path = nullptr;
  int opt;
  while ((opt = getopt(argc, argv, "hc:")) != -1) {
    switch (opt) {
    case 'c':
      config_path = optarg;
      break;
    case 'h':
    default:
      usage(argv[0]);
    }
  }
  if (config_path == nullptr) {
    usage(argv[0]);
  }

  try {
    do_simulation(config_path);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
