// Hello filesystem class definition

#pragma once

#include <string>

#include "Fuse.h"

/**
 * This class implements a simple readonly FUSE-based filesystem known as HelloFS.
 *
 * HelloFS presents a virtual file system with a single "hello" file that contains
 * a predefined string. The class overrides and defines the necessary filesystem
 * operations such as `getattr`, `readdir`, `open`, and `read`. Additionally, it
 * provides utility functions to manage paths and file content relevant to the
 * filesystem logic.
 *
 * Features:
 * - Implements basic FUSE operations like retrieving attributes (getattr),
 *   reading directories (readdir), and file I/O operations (open, read).
 * - Exposes an in-memory file system structure with a single root directory and
 *   one file (`helloPath()`) containing predefined content (`helloStr()`).
 * - Encapsulates the root directory path, file path, and file content as private
 *   members, accessed through dedicated getter functions.
 *
 * Note:
 * - This implementation is intended as a minimal example of a FUSE-based filesystem
 *   and is not designed to support complex operations or modifications of the file
 *   system.
 */
class HelloFS : public Fusepp::Fuse<HelloFS>
{
public:
  HelloFS() = default;

  ~HelloFS() override = default;

  /**
   * Retrieves attributes of a file or directory.
   *
   * This function implements the `getattr` operation for the HelloFS filesystem.
   * It is responsible for providing file attributes for given paths. The
   * function populates the `stBuffer` structure with the appropriate metadata
   * based on the specified `path`.
   *
   * @param path The file or directory path for which the attributes are to be retrieved.
   * @param stBuffer A pointer to a `struct stat` that will be populated with the file's attributes.
   * @param fuse_file_info Reserved for future use.
   *
   * @return 0 on success, or a negative error code (e.g., -ENOENT) if the path does not exist.
   *
   * Behavior:
   * - If the path matches the root directory (`rootPath()`), the function provides
   *   directory attributes.
   * - If the path matches the file path (`helloPath()`), the function provides
   *   file attributes, including the file's size equal to the length of the string
   *   from `helloStr()`.
   * - If the path does not match any known paths, the function returns -ENOENT.
   */
  static int getattr (const char *, struct stat *, struct fuse_file_info *);

  /**
   * Reads directory contents.
   *
   * This function implements the `readdir` operation for the HelloFS filesystem.
   * It populates the buffer with the entries for the given directory path. These
   * entries include `.` (current directory), `..` (parent directory), and any
   * additional entries specific to the filesystem implementation.
   *
   * @param path The directory path whose contents are to be listed.
   * @param buf A user-provided buffer where the directory entries will be filled.
   * @param filler A callback function to add entries to the buffer.
   * @param offset This parameter is unused in this implementation.
   * @param fi Reserved for future use.
   * @param flags Additional flags for the `readdir` operation.
   *
   * @return 0 on success, or a negative error code (e.g., -ENOENT) if the directory
   *         does not exist.
   *
   * Behavior:
   * - If the `path` matches the root directory (`rootPath()`), the function
   *   fills the buffer with `.` (current directory), `..` (parent directory),
   *   and the relative path of the `helloPath()` file.
   * - If the `path` does not match any known paths, the function returns -ENOENT.
   */
  static int readdir(const char *path, void *buf,
                     fuse_fill_dir_t filler,
                     off_t offset, struct fuse_file_info *fi,
                     enum fuse_readdir_flags);

  /**
   * Opens a file for read-only access.
   *
   * This function implements the `open` operation for the HelloFS filesystem.
   * It is invoked to verify the accessibility of the file and its mode. The
   * function ensures that the specified file exists and is opened with the
   * correct permissions.
   *
   * @param path The path to the file being opened.
   * @param fi A pointer to `struct fuse_file_info` containing information about
   *           the file access flags.
   *
   * @return 0 on success, or a negative error code:
   *         - -ENOENT if the file does not exist.
   *         - -EACCES if the file is not opened with read-only access.
   *
   * Behavior:
   * - If the `path` matches the `helloPath()` file, it validates the access flags.
   * - If the `path` does not match any known paths, the function returns -ENOENT.
   * - Only read-only access (`O_RDONLY`) is permitted; other modes return -EACCES.
   */
  static int open(const char *path, struct fuse_file_info *fi);

  /**
   * Reads data from a file in the HelloFS filesystem.
   *
   * This function implements the `read` operation for the HelloFS filesystem.
   * It reads a specified number of bytes from the file at the given offset and
   * stores them in the provided buffer. The file content is defined by the
   * `helloStr()` string of the filesystem.
   *
   * @param path The path to the file from which data will be read.
   * @param buf A pointer to the buffer where the read data will be stored.
   * @param size The number of bytes to read.
   * @param offset The offset from the beginning of the file where the read operation should start.
   * @param fuse_file_info Reserved for future use.
   *
   * @return The number of bytes read on success. Returns 0 if the offset is beyond the file's size,
   *         or a negative error code (e.g., -ENOENT) if the file does not exist.
   *
   * Behavior:
   * - If the `path` matches the file path (`helloPath()`), the function reads data from the
   *   `helloStr()` string starting at the specified offset, up to the requested size or the
   *   end of the string, whichever comes first.
   * - If the `path` does not match any known file path, the function returns -ENOENT.
   */
  static int read(const char *path, char *buf, size_t size, off_t offset,
                  struct fuse_file_info *fi);

  /**
   * Retrieves the root directory path of the filesystem.
   *
   * This function returns a constant reference to the internal string
   * representing the root directory path used by the HelloFS filesystem.
   *
   * @return A constant reference to the string representing the root directory path.
   *
   * Usage Context:
   * - The root path is typically used to identify and match operations specific
   *   to the root directory within the filesystem.
   */
  const std::string& rootPath() const { return root_path_; }

  /**
   * Retrieves the string content used in the HelloFS filesystem.
   *
   * This method returns a constant reference to the internal string, which represents
   * the content of the hello file within the filesystem.
   *
   * @return A constant reference to the internal string containing the hello file content.
   */
  const std::string& helloStr() const { return hello_str_; }

  /**
   * Retrieves the path of the hello file in the HelloFS filesystem.
   *
   * This function provides access to the internal `hello_path_` member. It represents
   * the file path to the HelloFS "hello" file used for various filesystem operations.
   *
   * @return A constant reference to a string representing the path of the HelloFS file.
   */
  const std::string& helloPath() const  { return hello_path_; }

private:
    std::string root_path_ = "/";
    std::string hello_str_ = "Hello World!\n";
    std::string hello_path_ = "/hello";
};
