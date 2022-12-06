#include <iostream>
#include <unistd.h>
#include <cstring>

#include <capitalism_simulator.hpp>

static void usage(const char* binary_name) {
  std::cerr << "\nUsage:\n" << binary_name
    << " -n *ensemble size* -t *duration* -s *sample_every*\n";
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  int size, duration, sampling;
  int opt;
  while ((opt = getopt(argc, argv, "n:t:s:")) != -1) {
    switch(opt) {
    case 'n':
      size = std::atoi(optarg);
      break;
    case 't':
      duration = std::atoi(optarg);
      break;
    case 's':
      sampling = std::atoi(optarg);
      break;
    default:
      usage(argv[0]);
    }
  }
  
  capitalism::Simulator sim{size};
  sim.run(duration, sampling);
}
