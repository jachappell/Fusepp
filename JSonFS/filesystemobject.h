#pragma once
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <map>
#include <string>

class FileSystemObject {
protected:
  std::string name;

public:
  std::string getName();
  void setName(std::string name);
  virtual void convert();

  friend std::ostream &operator<<(std::ostream &, const FileSystemObject &);
};
