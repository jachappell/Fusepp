#include "filesystemobject.h"
#include <iostream>
#include <string>

void FileSystemObject::setName(std::string name) { this->name = name; }

std::string FileSystemObject::getName() { return this->name; }

void FileSystemObject::convert() {
  std::cout << "add(FileSystemObject)" << std::endl;
}
