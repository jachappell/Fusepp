//==================================================================
/**
 *  FuseApp -- A simple C++ wrapper for the FUSE filesystem
 *
 *  Copyright (C) 2019 by James A. Chappell (rlrrlrll@gmail.com)
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

#ifndef __FUSE_APP_H__
#define __FUSE_APP_H__

#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 35
#endif

#include <fuse.h>
#include <cerrno>
#include <cstring>

namespace Fusepp
{
  template <class T> class Fuse 
  {
  public:
    auto run(int argc, char **argv)
    {
      return fuse_main(argc, argv, Operations(), this);
    }

    /** Get file attributes.
     *
     * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
     * ignored. The 'st_ino' field is ignored except if the 'use_ino'
     * mount option is given. In that case it is passed to userspace,
     * but libfuse and the kernel will still assign a different
     * inode for internal use (called the "nodeid").
     *
     * `fi` will always be NULL if the file is not currently open, but
     * may also be NULL if the file is open.
     */
    int getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Read the target of a symbolic link
     *
     * The buffer should be filled with a null terminated string.  The
     * buffer size argument includes the space for the terminating
     * null character.  If the linkname is too long to fit in the
     * buffer, it should be truncated.  The return value should be 0
     * for success.
     */
    int readlink(const char *path, char *buf, size_t size)
    {
      return -ENOSYS;
    }

    /** Create a file node
     *
     * This is called for creation of all non-directory, non-symlink
     * nodes.  If the filesystem defines a create() method, then for
     * regular files that will be called instead.
     */
    int mknod(const char *path, mode_t mode, dev_t dev)
    {
      return -ENOSYS;
    }

    /** Create a directory
     *
     * Note that the mode argument may not have the type specification
     * bits set, i.e. S_ISDIR(mode) can be false.  To obtain the
     * correct directory type bits use  mode|S_IFDIR
     * */
    int mkdir(const char *path, mode_t mode)
    {
      return -ENOSYS;
    }

    /** Remove a file */
    int unlink(const char *path)
    {
      return -ENOSYS;
    }

    /** Remove a directory */
    int rmdir(const char *path)
    {
      return -ENOSYS;
    }

    /** Create a symbolic link */
    int symlink(const char *target, const char *linkpath)
    {
      return -ENOSYS;
    }

    /** Rename a file
     *
     * *flags* may be `RENAME_EXCHANGE` or `RENAME_NOREPLACE`. If
     * RENAME_NOREPLACE is specified, the filesystem must not
     * overwrite *newname* if it exists and return an error
     * instead. If `RENAME_EXCHANGE` is specified, the filesystem
     * must atomically exchange the two files, i.e. both must
     * exist and neither may be deleted.
     */
    int rename(const char *oldpath, const char *newpath, unsigned int flags)
    {
      return -ENOSYS;
    }

    /** Create a hard link to a file */
    int link(const char *oldpath, const char *newpath)
    {
      return -ENOSYS;
    }

    /** Change the permission bits of a file
     *
     * `fi` will always be NULL if the file is not currenlty open, but
     * may also be NULL if the file is open.
     */
    int chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Change the owner and group of a file
     *
     * `fi` will always be NULL if the file is not currenlty open, but
     * may also be NULL if the file is open.
     *
     * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
     * expected to reset the setuid and setgid bits.
     */
    int chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Change the size of a file
     *
     * `fi` will always be NULL if the file is not currenlty open, but
     * may also be NULL if the file is open.
     *
     * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
     * expected to reset the setuid and setgid bits.
     */
    int truncate(const char *path, off_t off, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Open a file
     *
     * Open flags are available in fi->flags. The following rules
     * apply.
     *
     *  - Creation (O_CREAT, O_EXCL, O_NOCTTY) flags will be
     *    filtered out / handled by the kernel.
     *
     *  - Access modes (O_RDONLY, O_WRONLY, O_RDWR, O_EXEC, O_SEARCH)
     *    should be used by the filesystem to check if the operation is
     *    permitted.  If the ``-o default_permissions`` mount option is
     *    given, this check is already done by the kernel before calling
     *    open() and may thus be omitted by the filesystem.
     *
     *  - When writeback caching is enabled, the kernel may send
     *    read requests even for files opened with O_WRONLY. The
     *    filesystem should be prepared to handle this.
     *
     *  - When writeback caching is disabled, the filesystem is
     *    expected to properly handle the O_APPEND flag and ensure
     *    that each write is appending to the end of the file.
     * 
     *  - When writeback caching is enabled, the kernel will
     *    handle O_APPEND. However, unless all changes to the file
     *    come through the kernel this will not work reliably. The
     *    filesystem should thus either ignore the O_APPEND flag
     *    (and let the kernel handle it), or return an error
     *    (indicating that reliably O_APPEND is not available).
     *
     * Filesystem may store an arbitrary file handle (pointer,
     * index, etc) in fi->fh, and use this in other all other file
     * operations (read, write, flush, release, fsync).
     *
     * Filesystem may also implement stateless file I/O and not store
     * anything in fi->fh.
     *
     * There are also some flags (direct_io, keep_cache) which the
     * filesystem may set in fi, to change the way the file is opened.
     * See fuse_file_info structure in <fuse_common.h> for more details.
     *
     * If this request is answered with an error code of ENOSYS
     * and FUSE_CAP_NO_OPEN_SUPPORT is set in
     * `fuse_conn_info.capable`, this is treated as success and
     * future calls to open will also succeed without being send
     * to the filesystem process.
     *
     */
    int open(const char *path, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Read data from an open file
     *
     * Read should return exactly the number of bytes requested except
     * on EOF or error, otherwise the rest of the data will be
     * substituted with zeroes.   An exception to this is when the
     * 'direct_io' mount option is specified, in which case the return
     * value of the read system call will reflect the return value of
     * this operation.
     */
    int read(const char *path, char *buf, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Write data to an open file
     *
     * Write should return exactly the number of bytes requested
     * except on error.   An exception to this is when the 'direct_io'
     * mount option is specified (see read operation).
     *
     * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
     * expected to reset the setuid and setgid bits.
     */
    int write(const char *path, const char *buf, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Get file system statistics
     *
     * The 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
     */
    int statfs(const char *path, struct statvfs *buf)
    {
      return -ENOSYS;
    }

    /** Possibly flush cached data
     *
     * BIG NOTE: This is not equivalent to fsync().  It's not a
     * request to sync dirty data.
     *
     * Flush is called on each close() of a file descriptor, as opposed to
     * release which is called on the close of the last file descriptor for
     * a file.  Under Linux, errors returned by flush() will be passed to 
     * userspace as errors from close(), so flush() is a good place to write
     * back any cached dirty data. However, many applications ignore errors 
     * on close(), and on non-Linux systems, close() may succeed even if flush()
     * returns an error. For these reasons, filesystems should not assume
     * that errors returned by flush will ever be noticed or even
     * delivered.
     *
     * NOTE: The flush() method may be called more than once for each
     * open().  This happens if more than one file descriptor refers to an
     * open file handle, e.g. due to dup(), dup2() or fork() calls.  It is
     * not possible to determine if a flush is final, so each flush should
     * be treated equally.  Multiple write-flush sequences are relatively
     * rare, so this shouldn't be a problem.
     *
     * Filesystems shouldn't assume that flush will be called at any
     * particular point.  It may be called more times than expected, or not
     * at all.
     *
     * [close]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html
     */
    int flush(const char *path, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Release an open file
     *
     * Release is called when there are no more references to an open
     * file: all file descriptors are closed and all memory mappings
     * are unmapped.
     *
     * For every open() call there will be exactly one release() call
     * with the same flags and file handle.  It is possible to
     * have a file opened more than once, in which case only the last
     * release will mean, that no more reads/writes will happen on the
     * file.  The return value of release is ignored.
     */
    int release(const char *path, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Synchronize file contents
     *
     * If the datasync parameter is non-zero, then only the user data
     * should be flushed, not the meta data.
     */
    int fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Set extended attributes */
    int setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
    {
      return -ENOSYS;
    }

    /** Get extended attributes */
    int getxattr(const char *path, const char *name, char *value, size_t size)
    {
      return -ENOSYS;
    }

    /** List extended attributes */
    int listxattr(const char *path, char *list, size_t size)
    {
      return -ENOSYS;
    }

    /** Remove extended attributes */
    int removexattr(const char *path, const char *name)
    {
      return -ENOSYS;
    }

    /** Open directory
     *
     * Unless the 'default_permissions' mount option is given,
     * this method should check if opendir is permitted for this
     * directory. Optionally opendir may also return an arbitrary
     * filehandle in the fuse_file_info structure, which will be
     * passed to readdir, releasedir and fsyncdir.
     */
    int opendir(const char *path, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Read directory
     *
     * The filesystem may choose between two modes of operation:
     *
     * 1) The readdir implementation ignores the offset parameter, and
     * passes zero to the filler function's offset.  The filler
     * function will not return '1' (unless an error happens), so the
     * whole directory is read in a single readdir operation.
     *
     * 2) The readdir implementation keeps track of the offsets of the
     * directory entries.  It uses the offset parameter and always
     * passes non-zero offset to the filler function.  When the buffer
     * is full (or an error happens) the filler function will return
     * '1'.
     */
    int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
    {
      return -ENOSYS;
    }

    /** Release directory
     */
    int releasedir(const char *path, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Synchronize directory contents
     *
     * If the datasync parameter is non-zero, then only the user data
     * should be flushed, not the meta data
     */
    int fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /**
     * Initialize filesystem
     */
    void init(struct fuse_conn_info *conn, struct fuse_config *cfg)
    { }

    /**
     * Clean up filesystem
     *
     * Called on filesystem exit.
     */
    void destroy()
    { }

    /**
     * Check file access permissions
     *
     * This will be called for the access() system call.  If the
     * 'default_permissions' mount option is given, this method is not
     * called.
     *
     * This method is not called under Linux kernel versions 2.4.x
     */
    int access(const char *path, int mode)
    {
      return -ENOSYS;
    }

    /**
     * Create and open a file
     *
     * If the file does not exist, first create it with the specified
     * mode, and then open it.
     *
     * If this method is not implemented or under Linux kernel
     * versions earlier than 2.6.15, the mknod() and open() methods
     * will be called instead.
     */
    int create(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /**
     * Perform POSIX file locking operation
     *
     * The cmd argument will be either F_GETLK, F_SETLK or F_SETLKW.
     *
     * For the meaning of fields in 'struct flock' see the man page
     * for fcntl(2).  The l_whence field will always be set to
     * SEEK_SET.
     *
     * For checking lock ownership, the 'fuse_file_info->owner'
     * argument must be used.
     *
     * For F_GETLK operation, the library will first check currently
     * held locks, and if a conflicting lock is found it will return
     * information without calling this method.   This ensures, that
     * for local locks the l_pid field is correctly filled in.  The
     * results may not be accurate in case of race conditions and in
     * the presence of hard links, but it's unlikely that an
     * application would rely on accurate GETLK results in these
     * cases.  If a conflicting lock is not found, this method will be
     * called, and the filesystem may fill out l_pid by a meaningful
     * value, or it may leave this field zero.
     *
     * For F_SETLK and F_SETLKW the l_pid field will be set to the pid
     * of the process performing the locking operation.
     *
     * Note: if this method is not implemented, the kernel will still
     * allow file locking to work locally.  Hence it is only
     * interesting for network filesystems and similar.
     */
    int lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *buf)
    {
      return -ENOSYS;
    }

    /**
     * Change the access and modification times of a file with
     * nanosecond resolution
     *
     * This supersedes the old utime() interface.  New applications
     * should use this.
     *
     * `fi` will always be NULL if the file is not currenlty open, but
     * may also be NULL if the file is open.
     *
     * See the utimensat(2) man page for details.
     */
    int utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /**
     * Map block index within file to block index within device
     *
     * Note: This makes sense only for block device backed filesystems
     * mounted with the 'blkdev' option
     */
    int bmap(const char *path, size_t blocksize, uint64_t *idx)
    {
      return -ENOSYS;
    }

    /**
     * Ioctl
     *
     * flags will have FUSE_IOCTL_COMPAT set for 32bit ioctls in
     * 64bit environment.  The size and direction of data is
     * determined by _IOC_*() decoding of cmd.  For _IOC_NONE,
     * data will be NULL, for _IOC_WRITE data is out area, for
     * _IOC_READ in area and if both are set in/out area.  In all
     * non-NULL cases, the area is of _IOC_SIZE(cmd) bytes.
     *
     * If flags has FUSE_IOCTL_DIR then the fuse_file_info refers to a
     * directory file handle.
     *
     * Note : the unsigned long request submitted by the application
     * is truncated to 32 bits.
     */
    int ioctl(const char *path, unsigned int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data)
    {
      return -ENOSYS;
    }

    /**
     * Poll for IO readiness events
     *
     * Note: If ph is non-NULL, the client should notify
     * when IO readiness events occur by calling
     * fuse_notify_poll() with the specified ph.
     *
     * Regardless of the number of times poll with a non-NULL ph
     * is received, single notification is enough to clear all.
     * Notifying more times incurs overhead but doesn't harm
     * correctness.
     *
     * The callee is responsible for destroying ph with
     * fuse_pollhandle_destroy() when no longer in use.
     */
    int poll(const char *path, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp)
    {
      return -ENOSYS;
    }

    /** Write contents of buffer to an open file
     *
     * Similar to the write() method, but data is supplied in a
     * generic buffer.  Use fuse_buf_copy() to transfer data to
     * the destination.
     *
     * Unless FUSE_CAP_HANDLE_KILLPRIV is disabled, this method is
     * expected to reset the setuid and setgid bits.
     */
    int write_buf(const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /** Store data from an open file in a buffer
     *
     * Similar to the read() method, but data is stored and
     * returned in a generic buffer.
     *
     * No actual copying of data has to take place, the source
     * file descriptor may simply be stored in the buffer for
     * later data transfer.
     *
     * The buffer must be allocated dynamically and stored at the
     * location pointed to by bufp.  If the buffer contains memory
     * regions, they too must be allocated using malloc().  The
     * allocated memory will be freed by the caller.
     */
    int read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /**
     * Perform BSD file locking operation
     *
     * The op argument will be either LOCK_SH, LOCK_EX or LOCK_UN
     *
     * Nonblocking requests will be indicated by ORing LOCK_NB to
     * the above operations
     *
     * For more information see the flock(2) manual page.
     *
     * Additionally fi->owner will be set to a value unique to
     * this open file.  This same value will be supplied to
     * ->release() when the file is released.
     *
     * Note: if this method is not implemented, the kernel will still
     * allow file locking to work locally.  Hence it is only
     * interesting for network filesystems and similar.
     */
    int flock(const char *path, struct fuse_file_info *fi, int op)
    {
      return -ENOSYS;
    }

    /**
     * Allocates space for an open file
     *
     * This function ensures that required space is allocated for specified
     * file.  If this function returns success then any subsequent write
     * request to specified range is guaranteed not to fail because of lack
     * of space on the file system media.
     */
    int fallocate(const char *path, int mode, off_t off, off_t len, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }

    /**
     * Copy a range of data from one file to another
     *
     * Performs an optimized copy between two file descriptors without the
     * additional cost of transferring data through the FUSE kernel module
     * to user space (glibc) and then back into the FUSE filesystem again.
     *
     * In case this method is not implemented, glibc falls back to reading
     * data from the source and writing to the destination. Effectively
     * doing an inefficient copy of the data.
     */
    ssize_t copy_file_range (const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out, struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags)
    {
      return -ENOSYS;
    }

    /**
     * Find next data or hole after the specified offset
     */
    off_t lseek (const char *path, off_t off, int whence, struct fuse_file_info *fi)
    {
      return -ENOSYS;
    }


    Fuse()
    {
      memset (&operations_, 0, sizeof (struct fuse_operations));
      load_operations_();
    }

    // no copy
    Fuse(const Fuse&) = delete;
    Fuse& operator=(const Fuse&) = delete;

    ~Fuse() = default;

    auto Operations() { return &operations_; }

    static auto this_()
    {
      return static_cast<T*>(fuse_get_context()->private_data);
    }

  private:

    void load_operations_()
    {
      if (&T::getattr != &Fuse::getattr) {
        operations_.getattr = _getattr;
      }
      if (&T::readlink != &Fuse::readlink) {
        operations_.readlink = _readlink;
      }
      if (&T::mknod != &Fuse::mknod) {
        operations_.mknod = _mknod;
      }
      if (&T::mkdir != &Fuse::mkdir) {
        operations_.mkdir = _mkdir;
      }
      if (&T::unlink != &Fuse::unlink) {
        operations_.unlink = _unlink;
      }
      if (&T::rmdir != &Fuse::rmdir) {
        operations_.rmdir = _rmdir;
      }
      if (&T::symlink != &Fuse::symlink) {
        operations_.symlink = _symlink;
      }
      if (&T::rename != &Fuse::rename) {
        operations_.rename = _rename;
      }
      if (&T::link != &Fuse::link) {
        operations_.link = _link;
      }
      if (&T::chmod != &Fuse::chmod) {
        operations_.chmod = _chmod;
      }
      if (&T::chown != &Fuse::chown) {
        operations_.chown = _chown;
      }
      if (&T::truncate != &Fuse::truncate) {
        operations_.truncate = _truncate;
      }
      if (&T::open != &Fuse::open) {
        operations_.open = _open;
      }
      if (&T::read != &Fuse::read) {
        operations_.read = _read;
      }
      if (&T::write != &Fuse::write) {
        operations_.write = _write;
      }
      if (&T::statfs != &Fuse::statfs) {
        operations_.statfs = _statfs;
      }
      if (&T::flush != &Fuse::flush) {
        operations_.flush = _flush;
      }
      if (&T::release != &Fuse::release) {
        operations_.release = _release;
      }
      if (&T::fsync != &Fuse::fsync) {
        operations_.fsync = _fsync;
      }
      if (&T::setxattr != &Fuse::setxattr) {
        operations_.setxattr = _setxattr;
      }
      if (&T::getxattr != &Fuse::getxattr) {
        operations_.getxattr = _getxattr;
      }
      if (&T::listxattr != &Fuse::listxattr) {
        operations_.listxattr = _listxattr;
      }
      if (&T::removexattr != &Fuse::removexattr) {
        operations_.removexattr = _removexattr;
      }
      if (&T::opendir != &Fuse::opendir) {
        operations_.opendir = _opendir;
      }
      if (&T::readdir != &Fuse::readdir) {
        operations_.readdir = _readdir;
      }
      if (&T::releasedir != &Fuse::releasedir) {
        operations_.releasedir = _releasedir;
      }
      if (&T::fsyncdir != &Fuse::fsyncdir) {
        operations_.fsyncdir = _fsyncdir;
      }
      if (&T::init != &Fuse::init) {
        operations_.init = _init;
      }
      if (&T::destroy != &Fuse::destroy) {
        operations_.destroy = _destroy;
      }
      if (&T::access != &Fuse::access) {
        operations_.access = _access;
      }
      if (&T::create != &Fuse::create) {
        operations_.create = _create;
      }
      if (&T::lock != &Fuse::lock) {
        operations_.lock = _lock;
      }
      if (&T::utimens != &Fuse::utimens) {
        operations_.utimens = _utimens;
      }
      if (&T::bmap != &Fuse::bmap) {
        operations_.bmap = _bmap;
      }
      if (&T::ioctl != &Fuse::ioctl) {
        operations_.ioctl = _ioctl;
      }
      if (&T::poll != &Fuse::poll) {
        operations_.poll = _poll;
      }
      if (&T::write_buf != &Fuse::write_buf) {
        operations_.write_buf = _write_buf;
      }
      if (&T::read_buf != &Fuse::read_buf) {
        operations_.read_buf = _read_buf;
      }
      if (&T::flock != &Fuse::flock) {
        operations_.flock = _flock;
      }
      if (&T::fallocate != &Fuse::fallocate) {
        operations_.fallocate = _fallocate;
      }
    }

    struct fuse_operations operations_;

    static int _getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
    {
      return this_()->getattr(path, stbuf, fi);
    }

    static int _readlink(const char *path, char *buf, size_t size)
    {
      return this_()->readlink(path, buf, size);
    }

    static int _mknod(const char *path, mode_t mode, dev_t dev)
    {
      return this_()->mknod(path, mode, dev);
    }

    static int _mkdir(const char *path, mode_t mode)
    {
      return this_()->mkdir(path, mode);
    }

    static int _unlink(const char *path)
    {
      return this_()->unlink(path);
    }

    static int _rmdir(const char *path)
    {
      return this_()->rmdir(path);
    }

    static int _symlink(const char *target, const char *linkpath)
    {
      return this_()->symlink(target, linkpath);
    }

    static int _rename(const char *oldpath, const char *newpath, unsigned int flags)
    {
      return this_()->rename(oldpath, newpath, flags);
    }

    static int _link(const char *oldpath, const char *newpath)
    {
      return this_()->link(oldpath, newpath);
    }

    static int _chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
      return this_()->chmod(path, mode, fi);
    }

    static int _chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
    {
      return this_()->chown(path, uid, gid, fi);
    }

    static int _truncate(const char *path, off_t off, struct fuse_file_info *fi)
    {
      return this_()->truncate(path, off, fi);
    }

    static int _open(const char *path, struct fuse_file_info *fi)
    {
      return this_()->open(path, fi);
    }

    static int _read(const char *path, char *buf, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return this_()->read(path, buf, size, off, fi);
    }

    static int _write(const char *path, const char *buf, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return this_()->write(path, buf, size, off, fi);
    }

    static int _statfs(const char *path, struct statvfs *buf)
    {
      return this_()->statfs(path, buf);
    }

    static int _flush(const char *path, struct fuse_file_info *fi)
    {
      return this_()->flush(path, fi);
    }

    static int _release(const char *path, struct fuse_file_info *fi)
    {
      return this_()->release(path, fi);
    }

    static int _fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
      return this_()->fsync(path, datasync, fi);
    }

    static int _setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
    {
      return this_()->setxattr(path, name, value, size, flags);
    }

    static int _getxattr(const char *path, const char *name, char *value, size_t size)
    {
      return this_()->getxattr(path, name, value, size);
    }

    static int _listxattr(const char *path, char *list, size_t size)
    {
      return this_()->listxattr(path, list, size);
    }

    static int _removexattr(const char *path, const char *name)
    {
      return this_()->removexattr(path, name);
    }

    static int _opendir(const char *path, struct fuse_file_info *fi)
    {
      return this_()->opendir(path, fi);
    }

    static int _readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
    {
      return this_()->readdir(path, buf, filler, off, fi, flags);
    }

    static int _releasedir(const char *path, struct fuse_file_info *fi)
    {
      return this_()->releasedir(path, fi);
    }

    static int _fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
    {
      return this_()->fsyncdir(path, datasync, fi);
    }

    static void *_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
    {
      auto _this = this_();
      _this->init(conn, cfg);
      return _this;
    }

    static void _destroy(void *private_data)
    {
      return static_cast<T*>(private_data)->destroy();
    }

    static int _access(const char *path, int mode)
    {
      return this_()->access(path, mode);
    }

    static int _create(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
      return this_()->create(path, mode, fi);
    }

    static int _lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *buf)
    {
      return this_()->lock(path, fi, cmd, buf);
    }

    static int _utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi)
    {
      return this_()->utimens(path, tv, fi);
    }

    static int _bmap(const char *path, size_t blocksize, uint64_t *idx)
    {
      return this_()->bmap(path, blocksize, idx);
    }

  #if FUSE_USE_VERSION < 35
      #define _FUSE_CMD_SIGN
  #else
      #define _FUSE_CMD_SIGN unsigned
  #endif
    static int _ioctl(const char *path, _FUSE_CMD_SIGN int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data)
    {
      return this_()->ioctl(path, cmd, arg, fi, flags, data);
    }
  #undef CMD_SIGN

    static int _poll(const char *path, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp)
    {
      return this_()->poll(path, fi, ph, reventsp);
    }

    static int _write_buf(const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi)
    {
      return this_()->write_buf(path, buf, off, fi);
    }

    static int _read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi)
    {
      return this_()->read_buf(path, bufp, size, off, fi);
    }
    static int _flock(const char *path, struct fuse_file_info *fi, int op)
    {
      return this_()->flock(path, fi, op);
    }

    static int _fallocate(const char *path, int mode, off_t off, off_t len, struct fuse_file_info *fi)
    {
      return this_()->fallocate(path, mode, off, len, fi);
    }

    static ssize_t _copy_file_range(const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out, struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags)
    {
      return this_()->copy_file_range(path_in, fi_in, offset_in, path_out, fi_out, offset_out, size, flags);
    }

    static off_t _lseek(const char *path, off_t off, int whence, struct fuse_file_info *fi)
    {
      return this_()->lseek(path, off, whence, fi);
    }

  } ;
};

#endif
