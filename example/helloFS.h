// Hello filesystem class definition

#pragma once

#include <string>

#include "Fuse.h"

class HelloFS : public Fusepp::Fuse<HelloFS>
{
public:
  HelloFS() = default;

  ~HelloFS() = default;

  static int getattr (const char *, struct stat *, struct fuse_file_info *);

  static int readdir(const char *path, void *buf,
                     fuse_fill_dir_t filler,
                     off_t offset, struct fuse_file_info *fi,
                     enum fuse_readdir_flags);
  
  static int open(const char *path, struct fuse_file_info *fi);

  static int read(const char *path, char *buf, size_t size, off_t offset,
                  struct fuse_file_info *fi);

  const std::string& rootPath() const { return root_path_; }
  const std::string& helloStr() const { return hello_str_; }
  const std::string& helloPath() const  { return hello_path_; }

private:
    std::string root_path_ = "/";
    std::string hello_str_ = "Hello World!\n";
    std::string hello_path_ = "/hello";
};
