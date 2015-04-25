//==================================================================
/**
 *  FuseApp -- A simple C++ wrapper for the FUSE filesystem
 *
 *  Copyright (C) 2015 by James A. Chappell (rlrrlrll@gmail.com)
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
//=================================================================
/*
 * FileInfo.h: Version 0.01
 * Created by James A. Chappell
 * Created 25 April 2015
 *
 * History:
 * 25-apr-2015  created
 */
//==============

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

  std::string Name() const { return _name; }
  void Name(const std::string& name) { _name = name; }

  ino_t  Parent() const { return _parent; }
  void Parent(ino_t  parent) { _parent = parent; }

  uint64_t Data() const { return _data; }
  void Data(uint64_t data) { _data = data; }

  const struct stat *Stat() const { return &_fstat; }
  void Stat(const struct stat& fstat) { _fstat = fstat; }

  int IsDir() const { return S_ISDIR(_fstat.st_mode); } 

  ino_t Ino() const { return _fstat.st_ino; }

  static bool FetchFileInfo(const std::string& path, FileInfo& file_info);
  static void InsertFileInfo(const std::string& path, FileInfo& fi);
  static void DeleteFileInfo(const std::string& path);
  static void ClearFileInfo();

private:

  struct stat _fstat;
  std::string _name;
  ino_t _parent;
  uint64_t _data;

  static FileMap _files;
};

#endif
