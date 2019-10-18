#pragma once
#include "filesystemobject.h"
#include <iostream>
#include <map>
#include <string>

class File : public FileSystemObject {
private:
  std::string content;

public:
  std::string getContent();
  void setContent(std::string name);

  friend std::ostream &operator<<(std::ostream &, const File &);
};
