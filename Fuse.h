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
 * Fuse.h: Version 0.01
 * Created by James A. Chappell
 * Created 25 April 2015
 *
 * History:
 * 25-apr-2015  created
 */
//==============


#ifndef __FUSE_APP_H__
#define __FUSE_APP_H__

#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 26
#endif

#include <fuse.h>
#include <string.h>

#include <boost/noncopyable.hpp>

namespace Fusepp
{
  typedef int(*t_readlink)(const char *, char *, size_t);
  typedef int(*t_getattr)(const char *, struct stat *);
  typedef int(*t_getdir) (const char *, fuse_dirh_t, fuse_dirfil_t);
  typedef int(*t_mknod) (const char *, mode_t, dev_t);
  typedef int(*t_mkdir) (const char *, mode_t);
  typedef int(*t_unlink) (const char *);
  typedef int(*t_rmdir) (const char *);
  typedef int(*t_symlink) (const char *, const char *);
  typedef int(*t_rename) (const char *, const char *);
  typedef int(*t_link) (const char *, const char *);
  typedef int(*t_chmod) (const char *, mode_t);
  typedef int(*t_chown) (const char *, uid_t, gid_t);
  typedef int(*t_truncate) (const char *, off_t);
  typedef int(*t_utime) (const char *, struct utimbuf *);
  typedef int(*t_open) (const char *, struct fuse_file_info *);
  typedef int(*t_read) (const char *, char *, size_t, off_t,
                        struct fuse_file_info *);
  typedef int(*t_write) (const char *, const char *, size_t,
                         off_t,struct fuse_file_info *);
  typedef int(*t_statfs) (const char *, struct statvfs *);
  typedef int(*t_flush) (const char *, struct fuse_file_info *);
  typedef int(*t_release) (const char *, struct fuse_file_info *);
  typedef int(*t_fsync) (const char *, int, struct fuse_file_info *);
  typedef int(*t_setxattr) (const char *, const char *, const char *,
                            size_t, int);
  typedef int(*t_getxattr) (const char *, const char *, char *, size_t);
  typedef int(*t_listxattr) (const char *, char *, size_t);
  typedef int(*t_removexattr) (const char *, const char *);
  typedef int(*t_opendir) (const char *, struct fuse_file_info *);
  typedef int(*t_readdir) (const char *, void *, fuse_fill_dir_t, off_t,
                           struct fuse_file_info *);
  typedef int(*t_releasedir) (const char *, struct fuse_file_info *);
  typedef int(*t_fsyncdir) (const char *, int, struct fuse_file_info *);
  typedef void *(*t_init) (struct fuse_conn_info *);
  typedef void (*t_destroy) (void *);
  typedef int(*t_access) (const char *, int);
  typedef int(*t_create) (const char *, mode_t, struct fuse_file_info *);
  typedef int(*t_ftruncate) (const char *, off_t, struct fuse_file_info *);
  typedef int(*t_fgetattr) (const char *, struct stat *,
                            struct fuse_file_info *);

  template <class T> class Fuse : private boost::noncopyable
  {
  public:
    Fuse()
    {
      memset (&T::_operations, 0, sizeof (struct fuse_operations));
      load_operations();
    }

    int run(int argc, char **argv)
    {
      return fuse_main(argc, argv, Operations(), this);
    }

    static struct fuse_operations* Operations() { return &_operations; }

    static T* _this()
    {
      return static_cast<T*>(fuse_get_context()->private_data);
    }

  private:
      
    static void load_operations()
    {
      _operations.readlink = T::readlink;
      _operations.getattr = T::getattr;
      _operations.getdir = T::getdir;
      _operations.mknod = T::mknod;
      _operations.mkdir = T::mkdir;
      _operations.unlink = T::unlink;
      _operations.rmdir = T::rmdir;
      _operations.symlink = T::symlink;
      _operations.rename = T::rename;
      _operations.link = T::link;
      _operations.chmod = T::chmod;
      _operations.chown = T::chown;
      _operations.truncate = T::truncate;
      _operations.utime = T::utime;
      _operations.open = T::open;
      _operations.read = T::read;
      _operations.write = T::write;
      _operations.statfs = T::statfs;
      _operations.flush = T::flush;
      _operations.release = T::release;
      _operations.fsync = T::fsync;
      _operations.setxattr = T::setxattr;
      _operations.getxattr = T::getxattr;
      _operations.listxattr = T::listxattr;
      _operations.removexattr = T::removexattr;
      _operations.readdir = T::readdir;
      _operations.opendir = T::opendir;
      _operations.releasedir = T::releasedir;
      _operations.fsyncdir = T::fsyncdir;
      _operations.init = T::init;
      _operations.destroy = T::destroy;
      _operations.access = T::access;
      _operations.create = T::create;
      _operations.ftruncate = T::ftruncate;
      _operations.fgetattr = T::fgetattr;
    }

    static struct fuse_operations _operations;

    static t_getattr getattr ;
    static t_readlink readlink;
    static t_getdir getdir;
    static t_mknod mknod;
    static t_mkdir mkdir;
    static t_unlink unlink;
    static t_rmdir rmdir;
    static t_symlink symlink;
    static t_rename rename;
    static t_link link;
    static t_chmod chmod;
    static t_chown chown;
    static t_truncate truncate;
    static t_utime utime;
    static t_open open;
    static t_read read;
    static t_write write;
    static t_statfs statfs;
    static t_flush flush;
    static t_release release;
    static t_fsync fsync;
    static t_setxattr setxattr;
    static t_getxattr getxattr;
    static t_listxattr listxattr;
    static t_removexattr removexattr;
    static t_opendir opendir;
    static t_readdir readdir;
    static t_releasedir releasedir;
    static t_fsyncdir fsyncdir;
    static t_init init;
    static t_destroy destroy;
    static t_access access;
    static t_create create;
    static t_ftruncate ftruncate;
    static t_fgetattr  fgetattr;
  } ;
};

#endif