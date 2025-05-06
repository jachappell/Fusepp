// Hello filesystem class implementation

#include "helloFS.h"

int HelloFS::getattr(const char *path, struct stat *stBuffer, struct fuse_file_info *)
{
	auto ptr = this_();

	int res = 0;

	memset(stBuffer, 0, sizeof(struct stat));
	if (path == ptr->rootPath()) {
		stBuffer->st_mode = S_IFDIR | 0755;
		stBuffer->st_nlink = 2;
	} else if (path == ptr->helloPath()) {
		stBuffer->st_mode = S_IFREG | 0444;
		stBuffer->st_nlink = 1;
		stBuffer->st_size = static_cast<off_t>(ptr->helloStr().length());
	} else
		res = -ENOENT;

	return res;
}

int HelloFS::readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			               off_t, struct fuse_file_info *, enum fuse_readdir_flags)
{
	auto ptr = this_();

	if (path != ptr->rootPath())
		return -ENOENT;

	filler(buf, ".", nullptr, 0, FUSE_FILL_DIR_PLUS);
	filler(buf, "..", nullptr, 0, FUSE_FILL_DIR_PLUS);
	filler(buf, ptr->helloPath().c_str() + 1, nullptr, 0, FUSE_FILL_DIR_PLUS);

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
	auto ptr = this_();

	if (path != ptr->helloPath())
		return -ENOENT;

	auto len = ptr->helloStr().length();
	if (static_cast<size_t>(offset) < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, ptr->helloStr().c_str() + offset, size);
	} else
		size = 0;

	return static_cast<int>(size);
}
