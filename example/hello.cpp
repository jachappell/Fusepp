#include "helloFS.h"

int main(int argc, char *argv[])
{

  HelloFS fs;

  int status = fs.run(argc, argv);

  return status;
}
