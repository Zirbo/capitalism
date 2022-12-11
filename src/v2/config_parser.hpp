#pragma once

#include <map>
#include <string>

namespace capitalism {

typedef std::map<std::string, std::string> Config_section;

class Config {
public:
  Config() = default;
  void parse(const char *path);
  Config(const char *path) { parse(path); };

  std::string output_dir_name;
  int population;
  int duration;
  int sampling;
  std::map<std::string, std::pair<int, int>> subsets;

protected:
  std::map<std::string, Config_section> config;
  void read_config(std::istream &openconfig);
  void validate();
  void validate_main();
  void validate_subsets();
  void print();
};

} // namespace capitalism
