#include "FuseImpl.h"
#include <fuse3\fuse_common.h>
#include <fuse3\fuse.h>


using  std::chrono::system_clock;



FuseImpl::FuseImpl(FileSystemParam params) {
	ops = {
            Getattr,
            Readlink,
            Mknod,
            Mkdir,
            Unlink,
            Rmdir,
            Symlink,
            Rename,
            Link,
            Chmod,
            Chown,
            Truncate,
            Open,
            Read,
            Write,
            Statfs,
            Flush,
            Release,
            0, // fsync
            SetXattr,
            GetXattr,
            ListXattr,
            RemovXattr,
            OpenDir,
            Readdir,
            Releasedir,
            0, // fsyncdir
            Init,
            0, // destroy
            0, // access
            0, // create
            0, // lock
            Utimens,
            0, // bmap
#if 0
            ioctl,
#endif
    };
}

int
FuseImpl::Main(int argc, char* argv[], ThriftFuseClient* client) {
    FileSystemResponse response;
    _fsClient = client;
    return fuse_main(argc, argv, &ops, this);
}

FuseImpl* FuseImpl::GetInstance()
{
    return nullptr;
}

int FuseImpl::Getattr(const char* path, struct fuse_stat* stbuf, struct fuse_file_info* fi)
{
    return 0;
    /*
    auto node = client->GetAttributes(path, fi);
    if (!node)
        return -ENOENT;
    *stbuf = node->stat;
    return 0;
    */
}

int FuseImpl::Readlink(const char* path, char* buf, size_t size)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path);
    if (!node)
        return -ENOENT;
    if (S_IFLNK != (node->stat.st_mode & S_IFMT))
        return EINVAL;
    size = (std::min)(size - 1, node->data.size());
    std::memcpy(buf, node->data.data(), size);
    buf[size] = '\0';
    return 0;
    */
}

int FuseImpl::Mknod(const char* path, fuse_mode_t mode, fuse_dev_t dev)
{
    
    return 0;
}

int FuseImpl::Mkdir(const char* path, fuse_mode_t mode)
{
    return 0;
    /*
    auto self = getself();
    
    return self->make_node(path, S_IFDIR | (mode & 07777), 0);
    */
}

int FuseImpl::Unlink(const char* path)
{
    return 0;
    /*
    auto self = getself();
    
    return self->remove_node(path, false);
    */
}

int FuseImpl::Rmdir(const char* path)
{
    return 0;
    /*
    auto self = getself();
    
    return self->remove_node(path, true);
    */
}

int FuseImpl::Symlink(const char* dstpath, const char* srcpath)
{
    return 0;
    /*
    auto self = getself();
    
    return self->make_node(srcpath, S_IFLNK | 00777, 0, dstpath);
    */
}

int FuseImpl::Rename(const char* oldpath, const char* newpath, unsigned int flags)
{
    return 0;
    /*
    auto self = getself();
    
    auto oldlookup = self->lookup_node(oldpath);
    auto oldprnt = std::get<0>(oldlookup);
    auto oldname = std::get<1>(oldlookup);
    auto oldnode = std::get<2>(oldlookup);
    if (!oldnode)
        return -ENOENT;
    auto newlookup = self->lookup_node(newpath);
    auto newprnt = std::get<0>(newlookup);
    auto newname = std::get<1>(newlookup);
    auto newnode = std::get<2>(newlookup);
    if (!newprnt)
        return -ENOENT;
    if (newname.empty())
        // guard against directory loop creation
        return -EINVAL;
    if (oldprnt == newprnt && oldname == newname)
        return 0;
    if (newnode)
    {
        if (int errc = self->remove_node(newpath, S_IFDIR == (oldnode->stat.st_mode & S_IFMT)))
            return errc;
    }
    oldprnt->childmap.erase(oldname);
    newprnt->childmap[newname] = oldnode;
    return 0;
    */
}

int FuseImpl::Link(const char* oldpath, const char* newpath)
{
    return 0;
    /*
    auto self = getself();
    
    auto oldlookup = self->lookup_node(oldpath);
    auto oldnode = std::get<2>(oldlookup);
    if (!oldnode)
        return -ENOENT;
    auto newlookup = self->lookup_node(newpath);
    auto newprnt = std::get<0>(newlookup);
    auto newname = std::get<1>(newlookup);
    auto newnode = std::get<2>(newlookup);
    if (!newprnt)
        return -ENOENT;
    if (newnode)
        return -EEXIST;
    oldnode->stat.st_nlink++;
    newprnt->childmap[newname] = oldnode;
    oldnode->stat.st_ctim = newprnt->stat.st_ctim = newprnt->stat.st_mtim = now();
    return 0;
    */
}

int FuseImpl::Chmod(const char* path, fuse_mode_t mode,
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    node->stat.st_mode = (node->stat.st_mode & S_IFMT) | (mode & 07777);
    node->stat.st_ctim = now();
    return 0;
    */
}

int FuseImpl::Chown(const char* path, fuse_uid_t uid, fuse_gid_t gid,
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    if (-1 != uid)
        node->stat.st_uid = uid;
    if (-1 != gid)
        node->stat.st_gid = gid;
    node->stat.st_ctim = now();
    return 0;
    */
}

int FuseImpl::Truncate(const char* path, fuse_off_t size,
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    if (SIZE_MAX < size)
        return -EFBIG;
    node->resize(static_cast<size_t>(size), true);
    node->stat.st_ctim =
    node->stat.st_mtim = now();
    return 0;
    */
}

int FuseImpl::Open(const char* path, struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    return self->open_node(path, false, fi);
    */
}

int FuseImpl::Read(const char* path, char* buf, size_t size, fuse_off_t off,
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    fuse_off_t endoff = (std::min)(
        off + static_cast<fuse_off_t>(size), static_cast<fuse_off_t>(node->data.size()));
    if (off > endoff)
        return 0;
    std::memcpy(buf, node->data.data() + off, static_cast<int>(endoff - off));
    node->stat.st_atim = now();
    return static_cast<int>(endoff - off);
    */
}

int FuseImpl::Write(const char* path, const char* buf, size_t size, fuse_off_t off,
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    fuse_off_t endoff = off + static_cast<fuse_off_t>(size);
    if (SIZE_MAX < endoff)
        return -EFBIG;
    if (node->data.size() < endoff)
        node->resize(static_cast<size_t>(endoff), true);
    std::memcpy(node->data.data() + off, buf, static_cast<int>(endoff - off));
    node->stat.st_ctim = node->stat.st_mtim = now();
    return static_cast<int>(endoff - off);
    */
}

int FuseImpl::Statfs(const char* path, struct fuse_statvfs* stbuf)
{
    return 0;
    /*
    std::memset(stbuf, 0, sizeof * stbuf);
    return 0;
    */
}

int FuseImpl::Flush(const char* path, struct fuse_file_info* fi)
{
    return 0;
    /*
    return -ENOSYS;
    */
}

int FuseImpl::Release(const char* path, struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    return self->close_node(fi);
    */
}

int FuseImpl::SetXattr(const char* path, const char* name0, const char* value, size_t size,
    int flags)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path);
    if (!node)
        return -ENOENT;
    if (0 == std::strcmp("com.apple.ResourceFork", name0))
        return -ENOTSUP;
    std::string name = name0;
    if (XATTR_CREATE == flags)
    {
        if (node->xattrmap.end() != node->xattrmap.find(name))
            return -EEXIST;
    }
    else if (XATTR_REPLACE == flags)
    {
        if (node->xattrmap.end() == node->xattrmap.find(name))
            return -ENOATTR;
    }
    node->xattrmap[name].assign(value, value + size);
    return 0;
    */
}

int FuseImpl::GetXattr(const char* path, const char* name0, char* value, size_t size)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path);
    if (!node)
        return -ENOENT;
    if (0 == std::strcmp("com.apple.ResourceFork", name0))
        return -ENOTSUP;
    std::string name = name0;
    auto iter = node->xattrmap.find(name);
    if (node->xattrmap.end() == iter)
        return -ENOATTR;
    if (0 != size)
    {
        if (iter->second.size() > size)
            return -ERANGE;
        std::memcpy(value, iter->second.data(), iter->second.size());
    }
    return static_cast<int>(iter->second.size());
    */
}

int FuseImpl::ListXattr(const char* path, char* namebuf, size_t size)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path);
    if (!node)
        return -ENOENT;
    size_t copysize = 0;
    for (auto elem : node->xattrmap)
    {
        size_t namesize = elem.first.size() + 1;
        if (0 != size)
        {
            if (copysize + namesize > size)
                return -ERANGE;
            std::memcpy(namebuf + copysize, elem.first.c_str(), namesize);
            copysize += namesize;
        }
    }
    return static_cast<int>(copysize);
    */
}

int FuseImpl::RemovXattr(const char* path, const char* name0)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path);
    if (!node)
        return -ENOENT;
    if (0 == std::strcmp("com.apple.ResourceFork", name0))
        return -ENOTSUP;
    std::string name = name0;
    return node->xattrmap.erase(name) ? 0 : -ENOATTR;
    */
}

int FuseImpl::OpenDir(const char* path, struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    return self->open_node(path, true, fi);
    */
}

int FuseImpl::Readdir(const char* path, void* buf, fuse_fill_dir_t filler, fuse_off_t off,
    struct fuse_file_info* fi, enum fuse_readdir_flags)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    filler(buf, ".", &node->stat, 0, FUSE_FILL_DIR_PLUS);
    filler(buf, "..", nullptr, 0, FUSE_FILL_DIR_PLUS);
    for (auto elem : node->childmap)
        if (0 != filler(buf, elem.first.c_str(), &elem.second->stat, 0, FUSE_FILL_DIR_PLUS))
            break;
    return 0;
    */
}

int FuseImpl::Releasedir(const char* path, struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    return self->close_node(fi);
    */
}

static void* init(struct fuse_conn_info* conn,
    struct fuse_config* conf)
{
    return 0;
    /*
    conn->want |= (conn->capable & FUSE_CAP_READDIRPLUS);
    return getself();
    */
}

int FuseImpl::Utimens(const char* path, const struct fuse_timespec tmsp[2],
    struct fuse_file_info* fi)
{
    return 0;
    /*
    auto self = getself();
    
    auto node = self->get_node(path, fi);
    if (!node)
        return -ENOENT;
    if (tmsp)
    {
        node->stat.st_ctim = now();
        node->stat.st_atim = tmsp[0];
        node->stat.st_mtim = tmsp[1];
    }
    else
        node->stat.st_ctim = node->stat.st_atim = node->stat.st_mtim = now();
    return 0;
    */
}

void* FuseImpl::Init(struct fuse3_conn_info* conn, struct fuse3_config* conf) {
    return nullptr;
}