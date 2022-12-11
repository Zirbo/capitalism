#include <fstream>
#include <iostream>
#include <stdexcept>

#include "config_parser.hpp"

namespace capitalism {

void Config::parse(const char *path) {
  std::ifstream openconfig(path);
  if (!openconfig) {
    throw std::runtime_error("cannot find or read the config");
  }

  read_config(openconfig);
  validate();
}

static void trim(std::string &str) {
  auto pos = str.find_first_not_of(" \t");
  if (pos == std::string::npos) {
    // entire string empty
    str.erase();
  }
  str.erase(0, pos);

  pos = str.find_last_not_of(" \t");
  if (pos != std::string::npos) {
    str.erase(pos + 1);
  }
}

static void parse_normal_line(std::string &line,
                              Config_section &current_section) {
  // ignore comments
  auto pos = line.find_first_of('#');
  if (pos != std::string::npos) {
    line.erase(pos);
  }

  pos = line.find_first_of('=');
  if (pos != std::string::npos) {
    std::string key = line.substr(0, pos);
    trim(key);
    if (key.length() == 0) {
      return;
    }
    std::string value = line.substr(pos + 1);
    trim(value);

    current_section[key] = value;
  }
}

void Config::read_config(std::istream &openconfig) {
  std::string line;
  std::string current_section_name;
  Config_section current_section;
  while (openconfig.good()) {
    getline(openconfig, line);
    auto first_nonempty = line.find_first_not_of(" \t");
    if (first_nonempty == line.npos || line[first_nonempty] == '#') {
      // skip empty lines and comments
      continue;
    }
    if (line[first_nonempty] == '[') {
      // push current section
      if (!current_section_name.empty()) {
        // well, don't push if you have just started
        if (config.count(current_section_name) == 1) {
          throw std::runtime_error("duplicated section found in config");
        }
        config[current_section_name] = current_section;
        current_section.clear();
      }

      // start new section
      auto start_braket = first_nonempty + 1;
      auto len = line.find(']') - start_braket;
      current_section_name = line.substr(start_braket, len);
      trim(current_section_name);
    } else {
      if (config.count(current_section_name) == 1) {
        throw std::runtime_error("duplicated section found in config");
      }
      parse_normal_line(line, current_section);
    }
  }
  // push last section
  config[current_section_name] = current_section;
}

static void find_field(const Config_section &section, const std::string &name,
                       std::string &destination) {
  auto it = section.find(name);
  if (it == section.end()) {
    std::string msg = "field not found: " + name;
    throw std::runtime_error(msg);
  }
  destination = it->second;
}

static void find_field(const Config_section &section, const std::string &name,
                       int &destination) {
  auto it = section.find(name);
  if (it == section.end()) {
    std::string msg = "field not found: " + name;
    throw std::runtime_error(msg);
  }
  destination = std::atoi(it->second.c_str());
}

void Config::validate() {
  validate_main();
  validate_subsets();
  config.clear();
}

void Config::validate_main() {
  auto it = config.find("main");
  if (it == config.end()) {
    throw std::runtime_error("missing [main] section");
  }
  const auto &main_section = it->second;
  find_field(main_section, "output_dir_name", output_dir_name);
  find_field(main_section, "population", population);
  find_field(main_section, "duration", duration);
  find_field(main_section, "sampling", sampling);
}

void Config::validate_subsets() {
  auto subsects_section = config.find("subsets");
  if (subsects_section == config.end()) {
    throw std::runtime_error("missing [subsets] section");
  }

  int subsets_number = 0;
  int subsets_total_size = 0;
  for (const auto &sub_set : subsects_section->second) {
    Subset new_sub;
    new_sub.group = subsets_number;
    std::cout << "Representing " << sub_set.first << " as " << subsets_number
              << std ::endl;
    subsets_number++;
    const auto &csl = sub_set.second;
    auto comma = csl.find_first_of(',');
    if (comma == std::string::npos) {
      throw std::runtime_error("no comma found in subset");
    }
    new_sub.size = std::atoi(csl.substr(0, comma).c_str());
    new_sub.skill = std::atof(csl.substr(comma + 1).c_str());
    subsets_total_size += new_sub.size;
    subsets.push_back(new_sub);
  }

  if (subsets_total_size != population) {
    throw std::runtime_error("the subsets don't add to the total pop");
  }

  if (subsets_number != subsects_section->second.size()) {
    throw std::runtime_error("not all subsets traversed");
  }
}

} // namespace capitalism
