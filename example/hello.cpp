// See FUSE: example/hello.c

#include "helloFS.h"

// include in cpp file that instantiates HelloFS
#include "Fuse-impl.h"

int main(int argc, char *argv[])
{

  HelloFS fs;

  int status = fs.run(argc, argv);

  return status;
}
