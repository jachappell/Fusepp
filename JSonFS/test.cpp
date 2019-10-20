#include "io.h"
#include "directory.h"
#include "file.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "this is the beginning" << endl;

  Directory* root = load("test.json");
  root->convert();
//  cout << __LINE__ << root->getName() << endl;
/*
  File f;
  f.convert();
  FileSystemObject* fso = &f;
  fso->convert();
*/
//  save(root, "/tmp/foo.json");

  cout << "this is the ending" << endl;
}
