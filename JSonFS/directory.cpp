#include "directory.h"

std::map<std::string, FileSystemObject*> Directory::getContent() {
  return this->content;
}

void Directory::addEntry(FileSystemObject* fso) {
  this->content[fso->getName()] = fso;
}

void Directory::convert() {
  std::cout << "add(Directory)" << std::endl;
  for (auto elem : this->content) {
    std::cout << elem.first << std::endl;
    elem.second->convert();
  }
/*  
  boost::property_tree::ptree* directory = new boost::property_tree::ptree();
  directory->put(boost::property_tree::ptree::path_type{"name"}, this->getName());
  directory->put(boost::property_tree::ptree::path_type{"type"}, "directory");
  for (auto elem : this->content) {
    std::cout << elem.first << elem.second->getName() << std::endl;
    //boost::property_tree::ptree* child = elem.second->convert();
  }
  //directory.put(boost::property_tree::ptree::path_type{"content"}, this->content);
  return directory;
  */
}

std::ostream &operator<<(std::ostream &os, const Directory &obj) {
  os << obj.name << ": " << obj.content.size() << " Entr(y|ies)";
  return os;
}
