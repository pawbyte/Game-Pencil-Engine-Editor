// code from: https://codereview.stackexchange.com/questions/127819/ini-file-parser-in-c

#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <list>

// trim leading white-spaces
static std::string& ltrim(std::string& s) {
  size_t startpos = s.find_first_not_of(" \t\r\n\v\f");
  if (std::string::npos != startpos) {
    s = s.substr(startpos);
  }
  return s;
}

// trim trailing white-spaces
static std::string& rtrim(std::string& s) {
  size_t endpos = s.find_last_not_of(" \t\r\n\v\f");
  if (std::string::npos != endpos) {
    s = s.substr(0, endpos + 1);
  }
  return s;
}

config::config(const std::string& filename) {
  parse(filename);
}

section *config::get_section(const std::string& sectionname) {
  std::list<section>::iterator found = std::find_if(sections.begin(), sections.end(), [sectionname](const section& sect) { 
    return sect.name.compare(sectionname) == 0; });

  return found != sections.end() ? &*found : NULL;
}

std::list<section>& config::get_sections() {
  return sections;
}

std::string config::get_value(const std::string& sectionname, const std::string&keyname) {
  section *sect = get_section(sectionname);
  if (sect != NULL) {
    std::unordered_map<std::string, std::string>::const_iterator it = sect->keyvalues.find(keyname);
    if (it != sect->keyvalues.end())
      return it->second;
  }
  return "";
}

void config::parse(const std::string& filename) {
  section currentsection;
  std::ifstream fstrm;
  fstrm.open(filename);

  if (!fstrm)
    throw std::invalid_argument(filename + " could not be opened");

  for (std::string line; std::getline(fstrm, line);) {
    // if a comment
    if (!line.empty() && (line[0] == ';' || line[0] == '#')) {
      // allow both ; and # comments at the start of a line

    }
    else if (line[0] == '[') {
      /* A "[section]" line */
      size_t endpos = line.find_first_of(']');
      if (endpos != std::string::npos) {

        // this is a new section so if we have a current section populated, add it to list
        if (!currentsection.name.empty()) {
          sections.push_back(currentsection);  // copy
          currentsection.name.clear();  // clear section for re-use
          currentsection.keyvalues.clear();
        }
        currentsection.name = line.substr(1, endpos - 1);
      } else {
        // section has no closing ] char
      }
    }
    else if (!line.empty()) {
      /* Not a comment, must be a name[=:]value pair */
      size_t endpos = line.find_first_of("=:");
      if (endpos != std::string::npos) {
        std::string name = line.substr(0, endpos);
        std::string value = line.substr(endpos + 1);
        ltrim(rtrim(name));
        ltrim(rtrim(value));

        currentsection.keyvalues[name] = value;

    } else {
        // no key value delimitter
      }
    }
  } // for


  // if we are out of loop we add last section
  // this is a new section so if we have a current section populated, add it to list
  if (!currentsection.name.empty()) {
    sections.push_back(currentsection);  // copy
    currentsection.name = "";
    currentsection.keyvalues.clear();
  }
}
