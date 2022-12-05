#include <capitalism_simulator.hpp>

int main() {
  capitalism::Simulator sim{100, 100., 100.};
  sim.run(100, 25);
}
