#include "io.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "this is the beginning" << endl;

  Directory root = load("test.json");
  cout << root << endl;

  save(root, "/tmp/foo.json");

  cout << "this is the ending" << endl;
}
