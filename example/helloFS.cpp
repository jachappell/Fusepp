// Hello filesystem class implementation

#include "helloFS.h"

// include in one .cpp file
#include "Fuse-impl.h"

int HelloFS::getattr(const char *path, struct stat *stBuffer, struct fuse_file_info *)
{
	int res = 0;

	memset(stBuffer, 0, sizeof(struct stat));
	if (path == this_()->rootPath()) {
		stBuffer->st_mode = S_IFDIR | 0755;
		stBuffer->st_nlink = 2;
	} else if (path == this_()->helloPath()) {
		stBuffer->st_mode = S_IFREG | 0444;
		stBuffer->st_nlink = 1;
		stBuffer->st_size = static_cast<off_t>(this_()->helloStr().length());
	} else
		res = -ENOENT;

	return res;
}

int HelloFS::readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			               off_t, struct fuse_file_info *, enum fuse_readdir_flags)
{
	if (path != this_()->rootPath())
		return -ENOENT;

	filler(buf, ".", nullptr, 0, FUSE_FILL_DIR_PLUS);
	filler(buf, "..", nullptr, 0, FUSE_FILL_DIR_PLUS);
	filler(buf, this_()->helloPath().c_str() + 1, nullptr, 0, FUSE_FILL_DIR_PLUS);

	return 0;
}


int HelloFS::open(const char *path, struct fuse_file_info *fi)
{
	if (path != this_()->helloPath())
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}


int HelloFS::read(const char *path, char *buf, size_t size, off_t offset,
		              struct fuse_file_info *)
{
	if (path != this_()->helloPath())
		return -ENOENT;

	auto len = this_()->helloStr().length();
	if (static_cast<size_t>(offset) < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, this_()->helloStr().c_str() + offset, size);
	} else
		size = 0;

	return static_cast<int>(size);
}
