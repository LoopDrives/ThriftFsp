#include <cerrno>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <fuse3/fuse.h>
#include <ThriftFuse.h>

#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/PlatformSocket.h>

#include <thrift/protocol/TBinaryProtocol.h>



#include "Compact.h"

using namespace thriffuse;

typedef struct FileSystemParam {

};

#pragma once
class FuseImpl
{
private:
	// Thrif Remote Client
	ThriftFuseClient *_fsClient;

	// File System ctor
	FuseImpl(FileSystemParam fsParam);
	
	// Operation 
	fuse_operations ops;

	

	// Fuse interface implmentation 
	static int Getattr(const char* path, fuse_stat* stbuf, fuse_file_info* fi);
	static int Readlink(const char* path, char* buf, size_t size);
	static int Mknod(const char* path, fuse_mode_t mode, fuse_dev_t dev);
	static int Mkdir(const char* path, fuse_mode_t mode);
	static int Unlink(const char* path);
	static int Rmdir(const char* path);
	static int Symlink(const char* dstpath, const char* srcpath);
	static int Rename(const char* oldpath, const char* newpath, unsigned int flags);
	static int Link(const char* oldpath, const char* newpath);
	static int Chmod(const char* path, fuse_mode_t mode, fuse_file_info* fi);
	static int Chown(const char* path, fuse_uid_t uid, fuse_gid_t gid, fuse_file_info* fi);
	static int Truncate(const char* path, fuse_off_t size, fuse_file_info* fi);
	static int Open(const char* path, fuse_file_info* fi);
	static int Read(const char* path, char* buf, size_t size, fuse_off_t off, fuse_file_info* fi);
	static int Write(const char* path, const char* buf, size_t size, fuse_off_t off, fuse_file_info* fi);
	static int Statfs(const char* path, fuse_statvfs* stbuf);
	static int Flush(const char* path, fuse_file_info* fi);
	static int Release(const char* path, fuse_file_info* fi);
	static int SetXattr(const char* path, const char* name0, const char* value, size_t size, int flags);
	static int GetXattr(const char* path, const char* name0, char* value, size_t size);
	static int ListXattr(const char* path, char* namebuf, size_t size);
	static int RemovXattr(const char* path, const char* name0);
	static int OpenDir(const char* path, fuse_file_info* fi);
	static int Readdir(const char* path, void* buf, fuse_fill_dir_t filler, fuse_off_t off, fuse_file_info* fi, fuse_readdir_flags);
	static int Releasedir(const char* path, fuse_file_info* fi);
	static int Utimens(const char* path, const fuse_timespec tmsp[2], fuse_file_info* fi);
	static void* Init(fuse3_conn_info* conn, fuse3_config* conf);

public:
	// File System entry point
	int Main(int argc, char* argv[], ThriftFuseClient* client);
	static FuseImpl* GetInstance();
};

