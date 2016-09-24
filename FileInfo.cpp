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
FileInfo::FileMap FileInfo::files_;


FileInfo::FileInfo()
  : parent_(0)
  , data_(0)
{
  memset(&fstat_, 0, sizeof (struct stat));
}


FileInfo::FileInfo(const struct stat &fstat, ino_t parent, uint64_t data)
  : parent_(parent)
  , data_(data)
{
  Stat(fstat);
}


bool FileInfo::FetchFileInfo(const string& path, FileInfo& fi)
{
  bool found(false);
    
  FileMap::iterator it;

  it = files_.find(path);
  if (it != files_.end())
  {
    fi = it->second;

    found = ((fi.time_to_refresh_ == 0) || ((time(nullptr) - fi.insertion_time_) <= fi.time_to_refresh_));
  }

  return found;
}


void FileInfo::InsertFileInfo(const string& path, FileInfo& fi, time_t time_to_refresh)
{
  Lock lock;

  fi.insertion_time_ = time(nullptr);
  fi.time_to_refresh_ = time_to_refresh;

  files_[path] = fi;
}


void FileInfo::DeleteFileInfo(const string& path)
{
  Lock lock;
    
  FileMap::iterator it;

  it = files_.find(path);
  if (it != files_.end())
  {
    files_.erase(it);
  }
}


void FileInfo::ClearFileInfo()
{
  Lock lock;
    
  files_.clear();
}
