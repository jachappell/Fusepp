// Hello filesystem class definition

#ifndef __HELLOFS_H_
#define __HELLOFS_H_

#include "Fuse.h"

class HelloFS : public Fusepp::Fuse<HelloFS>
{
public:
  HelloFS() {}

  ~HelloFS() {}

  int getattr (const char *, struct stat *, struct fuse_file_info *);

  int readdir(const char *path, void *buf,
              fuse_fill_dir_t filler,
              off_t offset, struct fuse_file_info *fi,
              enum fuse_readdir_flags);
  
  int open(const char *path, struct fuse_file_info *fi);

  int read(const char *path, char *buf, size_t size, off_t offset,
           struct fuse_file_info *fi);
};

#endif
