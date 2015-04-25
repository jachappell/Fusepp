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
 * FileInfo.cpp: Version 0.01
 * Created by James A. Chappell
 * Created 25 April 2015
 *
 * History:
 * 25-apr-2015  created
 */
//==============


#include "File.h"
#include "FileInfo.h"

#include <pthread.h>

#include <boost/noncopyable.hpp>

using namespace std;

class Mutex : private boost::noncopyable
{
public:
  Mutex()
  {
    pthread_mutex_init(&_mutex, nullptr);
  }

  ~Mutex()
  {
    pthread_mutex_destroy(&_mutex);
  }

  pthread_mutex_t* Get() { return &_mutex; }

private:
  pthread_mutex_t _mutex;
};

class Lock : private boost::noncopyable
{
public:
  Lock() { pthread_mutex_lock(_mutex.Get()); }
  ~Lock() { pthread_mutex_unlock(_mutex.Get()); }

private:
  static Mutex _mutex;
};


Mutex Lock::_mutex;
FileInfo::FileMap FileInfo::_files;


FileInfo::FileInfo()
  : _parent(0)
  , _data(0)
{
  memset(&_fstat, 0, sizeof (struct stat));
}


FileInfo::FileInfo(const struct stat &fstat, ino_t parent, uint64_t data)
  : _parent(parent)
  , _data(data)
{
  Stat(fstat);
}


bool FileInfo::FetchFileInfo(const string& path, FileInfo& file_info)
{
  bool found(false);
    
  FileMap::iterator it;

  it = _files.find(path);
  if (it != _files.end())
  {
    file_info = it->second;
    found = true;
  }

  return found;
}


void FileInfo::InsertFileInfo(const string& path, FileInfo& fi)
{
  Lock lock;

  _files[path] = fi;
}


void FileInfo::DeleteFileInfo(const string& path)
{
  Lock lock;
    
  FileMap::iterator it;

  it = _files.find(path);
  if (it != _files.end())
  {
    _files.erase(it);
  }
}


void FileInfo::ClearFileInfo()
{
  Lock lock;
    
  _files.clear();
}
