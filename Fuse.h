//==================================================================
/**
 *  FuseApp -- A simple C++ wrapper for the FUSE filesystem
 *
 *  Copyright (C) 2025 by James A. Chappell (rlrrlrll@gmail.com)
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

#pragma once

#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 35
#endif

#include <fuse.h>
#include <cstring>

namespace Fusepp
{
  typedef int(*t_getattr)(const char *, struct stat *, struct fuse_file_info *);
  typedef int(*t_readlink)(const char *, char *, size_t);
  typedef int(*t_mknod) (const char *, mode_t, dev_t);
  typedef int(*t_mkdir) (const char *, mode_t);
  typedef int(*t_unlink) (const char *);
  typedef int(*t_rmdir) (const char *);
  typedef int(*t_symlink) (const char *, const char *);
  typedef int(*t_rename) (const char *, const char *,  unsigned int);
  typedef int(*t_link) (const char *, const char *);
  typedef int(*t_chmod) (const char *, mode_t, struct fuse_file_info *);
  typedef int(*t_chown) (const char *, uid_t, gid_t, fuse_file_info *);
  typedef int(*t_truncate) (const char *, off_t, fuse_file_info *);
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
                           struct fuse_file_info *, enum fuse_readdir_flags);
  typedef int(*t_releasedir) (const char *, struct fuse_file_info *);
  typedef int(*t_fsyncdir) (const char *, int, struct fuse_file_info *);
  typedef void *(*t_init) (struct fuse_conn_info *, struct fuse_config *cfg);
  typedef void (*t_destroy) (void *);
  typedef int(*t_access) (const char *, int);
  typedef int(*t_create) (const char *, mode_t, struct fuse_file_info *);
  typedef int(*t_lock) (const char *, struct fuse_file_info *, int cmd,
                        struct flock *);
  typedef int(*t_utimens) (const char *, const struct timespec tv[2],
                            struct fuse_file_info *fi);
  typedef int(*t_bmap) (const char *, size_t blocksize, uint64_t *idx);

#if FUSE_USE_VERSION < 35
  typedef int(*t_ioctl) (const char *, int cmd, void *arg,
#else
  typedef int(*t_ioctl) (const char *, unsigned int cmd, void *arg,
#endif
                         struct fuse_file_info *, unsigned int flags,
                         void *data);
  typedef int(*t_poll) (const char *, struct fuse_file_info *,
                        struct fuse_pollhandle *ph, unsigned *reventsp);
	typedef int(*t_write_buf) (const char *, struct fuse_bufvec *buf, off_t off,
                             struct fuse_file_info *);
  typedef int(*t_read_buf) (const char *, struct fuse_bufvec **bufp,
                            size_t size, off_t off, struct fuse_file_info *);
  typedef int (*t_flock) (const char *, struct fuse_file_info *, int op);
  typedef int (*t_fallocate) (const char *, int, off_t, off_t,
                              struct fuse_file_info *);

  /**
   * @class Fuse
   * @brief A template class that defines a framework for creating a FUSE-based filesystem.
   *
   * This class encapsulates the functionality needed to integrate with the FUSE (Filesystem in Userspace) library
   * and provides a structure to define callback operations for interacting with a custom filesystem.
   *
   * @tparam T The derived class implementing the specific filesystem logic, which must include the required
   * static callback operations used during filesystem events (e.g., getattr, mkdir, open, etc.).
   *
   * @note The class uses static operations and forbids copying or moving to maintain the integrity
   * of the FUSE library's operational model.
   */
  template <class T> class Fuse
  {
  public:
    /**
     * @brief Constructor for the Fuse class.
     *
     * Initializes the `fuse_operations` structure to zero and
     * loads the operations for the FUSE-based filesystem.
     *
     * @return A default-constructed instance of the Fuse class.
     */
    Fuse()
    {
      memset (&T::operations_, 0, sizeof (struct fuse_operations));
      load_operations_();
    }

    // no copy
    Fuse(const Fuse&) = delete;
    Fuse(Fuse&&) = delete;
    Fuse& operator=(const Fuse&) = delete;
    Fuse& operator= (Fuse&&) = delete;

    /**
     * @brief Virtual destructor for the Fuse class.
     *
     * Ensures proper cleanup of the derived class in scenarios where
     * the Fuse class is used as a base class.
     *
     * Defaulted to indicate no custom destructor logic is required.
     */
    virtual ~Fuse() = default;

    /**
     * @brief Starts the FUSE main loop for the filesystem.
     *
     * This function initializes and runs the FUSE event loop, utilizing the
     * filesystem operations provided by the `Operations` function. The loop
     * continues until the filesystem is unmounted or terminated.
     *
     * @param argc The number of command-line arguments passed to the program.
     * @param argv An array of strings containing the command-line arguments.
     * @return An integer status code returned by `fuse_main`. A value of `0` indicates
     *         the successful execution of the FUSE main loop, while a non-zero value
     *         indicates an error or abnormal termination.
     * Note:
     * - This function is a virtual function that can be overridden by derived classes
     *   to provide custom initialization logic.
     * - The default implementation calls `fuse_main` with the `Operations` function
     *   and the current instance of the class.
    *  - The overridden function should insure fuse_main with the `Operations` function,
     *   and the current instance of the class is called.
     */
    virtual int run(int argc, char **argv)
    {
      return fuse_main(argc, argv, Operations(), this);
    }

    /**
     * @brief Provides access to the static FUSE operations structure.
     *
     * This function returns a pointer to the `operations_` structure, which contains
     * the static definitions of file system operations implemented by the FUSE framework.
     *
     * @return A pointer to the `operations_` structure of type `fuse_operations`.
     */
    static auto Operations() { return &operations_; }

    /**
     * @brief Retrieves a pointer to the private data associated with the current FUSE context.
     *
     * This function casts the `private_data` pointer from the FUSE context to the expected class type `T*`,
     * allowing access to the user-defined private data object associated with the filesystem instance.
     *
     * @tparam T The class type to which the `private_data` is cast.
     * @return A pointer of type `T*` referring to the user-defined private data object of the filesystem.
     */
    static auto this_()
    {
      return static_cast<T*>(fuse_get_context()->private_data);
    }

  private:
      
    static void load_operations_()
    {
      operations_.getattr = T::getattr;
      operations_.readlink = T::readlink;
      operations_.mknod = T::mknod;
      operations_.mkdir = T::mkdir;
      operations_.unlink = T::unlink;
      operations_.rmdir = T::rmdir;
      operations_.symlink = T::symlink;
      operations_.rename = T::rename;
      operations_.link = T::link;
      operations_.chmod = T::chmod;
      operations_.chown = T::chown;
      operations_.truncate = T::truncate;
      operations_.open = T::open;
      operations_.read = T::read;
      operations_.write = T::write;
      operations_.statfs = T::statfs;
      operations_.flush = T::flush;
      operations_.release = T::release;
      operations_.fsync = T::fsync;
      operations_.setxattr = T::setxattr;
      operations_.getxattr = T::getxattr;
      operations_.listxattr = T::listxattr;
      operations_.removexattr = T::removexattr;
      operations_.opendir = T::opendir;
      operations_.readdir = T::readdir;
      operations_.releasedir = T::releasedir;
      operations_.fsyncdir = T::fsyncdir;
      operations_.init = T::init;
      operations_.destroy = T::destroy;
      operations_.access = T::access;
      operations_.create = T::create;
      operations_.lock = T::lock;
      operations_.utimens = T::utimens;
      operations_.bmap = T::bmap;
      operations_.ioctl = T::ioctl;
      operations_.poll = T::poll;
      operations_.write_buf = T::write_buf;
      operations_.read_buf = T::read_buf;
      operations_.flock = T::flock;
      operations_.fallocate = T::fallocate;
    }

    static struct fuse_operations operations_;

    static t_getattr getattr ;
    static t_readlink readlink;
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
    static t_lock lock;
    static t_utimens utimens;
    static t_bmap bmap;
    static t_ioctl ioctl;
    static t_poll poll;
    static t_write_buf write_buf;
    static t_read_buf read_buf;
    static t_flock flock;
    static t_fallocate fallocate;
  } ;
};

