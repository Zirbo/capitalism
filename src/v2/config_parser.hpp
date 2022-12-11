#pragma once

#include <map>
#include <string>
#include <vector>

namespace capitalism {

typedef std::map<std::string, std::string> Config_section;
struct Subset {
  int group;
  int size;
  double skill;
};

class Config {
public:
  Config() = default;
  void parse(const char *path);
  Config(const char *path) { parse(path); };

  std::string output_dir_name;
  int population;
  int duration;
  int sampling;
  std::vector<Subset> subsets;

protected:
  std::map<std::string, Config_section> config;
  void read_config(std::istream &openconfig);
  void validate();
  void validate_main();
  void validate_subsets();
};

} // namespace capitalism
