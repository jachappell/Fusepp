#include "directory.h"

std::map<std::string, FileSystemObject> Directory::getContent() {
  return this->content;
}

void Directory::addEntry(FileSystemObject fso) {
  this->content[fso.getName()] = fso;
}

std::ostream &operator<<(std::ostream &os, const Directory &obj) {
  os << obj.name << ": " << obj.content.size() << " Entr(y|ies)";
  return os;
}
