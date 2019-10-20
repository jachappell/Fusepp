#include "file.h"

void File::setContent(std::string content) { this->content = content; }

std::string File::getContent() { return this->content; }

void File::convert() {
  std::cout << "add(File)" << std::endl;
  /*
  boost::property_tree::ptree* file = new boost::property_tree::ptree();
  file->put(boost::property_tree::ptree::path_type{"name"}, this->getName());
  file->put(boost::property_tree::ptree::path_type{"type"}, "file");
  file->put(boost::property_tree::ptree::path_type{"content"}, this->content);
  return file;
  */
}

std::ostream &operator<<(std::ostream &os, const File &obj) {
  os << obj.name << ": " << obj.content.size() << " byte(s)";
  return os;
}
