#include "file.h"

void File::setContent(std::string content) { this->content = content; }

std::string File::getContent() { return this->content; }

std::ostream &operator<<(std::ostream &os, const File &obj) {
  os << obj.name << ": " << obj.content.size() << " byte(s)";
  return os;
}
