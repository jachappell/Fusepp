//==================================================================
/**
 *  FuseApp -- A simple C++ wrapper for the FUSE filesystem
 *
 *  Copyright (C) 2016 by James A. Chappell (rlrrlrll@gmail.com)
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  condition:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#include <sys/stat.h>
#include <string.h>

#include <string>
#include <map>

class FileInfo
{
public:

typedef std::map<std::string, FileInfo> FileMap;

  FileInfo();

  FileInfo(const struct stat &fstat, ino_t parent = 0,
           uint64_t data = 0);

  std::string Name() const { return name_; }
  void Name(const std::string& name) { name_ = name; }

  ino_t  Parent() const { return parent_; }
  void Parent(ino_t  parent) { parent_ = parent; }

  uint64_t Data() const { return data_; }
  void Data(uint64_t data) { data = data_; }

  const struct stat *Stat() const { return &fstat_; }
  void Stat(const struct stat& fstat) { fstat_ = fstat; }

  int IsDir() const { return S_ISDIR(fstat_.st_mode); } 

  ino_t Ino() const { return fstat_.st_ino; }

  static bool FetchFileInfo(const std::string& path, FileInfo& fi);
  static void InsertFileInfo(const std::string& path, FileInfo& fi, time_t time_to_refresh_ = 0);
  static void DeleteFileInfo(const std::string& path);
  static void ClearFileInfo();

private:

  struct stat fstat_;
  std::string name_;
  ino_t parent_;
  uint64_t data_;
  time_t insertion_time_;
  time_t time_to_refresh_;

  static FileMap files_;
};

#endif
