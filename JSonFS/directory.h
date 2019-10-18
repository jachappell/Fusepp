#pragma once
#include "filesystemobject.h"
#include <iostream>
#include <map>
#include <string>

class Directory : public FileSystemObject {
private:
  std::map<std::string, FileSystemObject*> content;

public:
  std::map<std::string, FileSystemObject*> getContent();
  void addEntry(FileSystemObject* fso);
  void convert();

  friend std::ostream &operator<<(std::ostream &, const Directory &);
};
