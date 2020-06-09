namespace cpp thriffuse 

struct FileInfo {


}

struct Permission {

}


struct OwenershipInfo {

}

enum StatusCode {
       None=0
}

struct FuseConnInfo {

}

struct KeyValuePair {
    1:string key,
    2:string val
}

typedef list<KeyValuePair> KVList


struct FileLock{

}

struct FileTimeSpec {
   1:i64 accessTime;
   2:i64 modificationTime;
}

struct FileSystemResponse {
    1: required StatusCode ret;
    2: optional FileInfo info;

}


service ThriftFuse {
  
   /*
    * getattr(const char* path, struct stat* stbuf)
    * Return file attributes. The "stat" structure is described in detail in the stat(2) manual page. 
    * For the given pathname, this should fill in the elements of the "stat" structure. If a field is 
    * meaningless or semi-meaningless (e.g., st_ino) then it should be set to 0 or given a "reasonable" value. 
    * This call is pretty much required for a usable filesystem.
    */
   FileSystemResponse GetAtrributes(1:string path, 2:FileInfo info);
  
   /*
    * readlink(const char* path, char* buf, size_t size)
    * If path is a symbolic link, fill buf with its target, up to size. 
    * See readlink(2) for how to handle a too-small buffer and for error codes. Not required if you don't support symbolic links. 
    * NOTE: Symbolic-link support requires only readlink and symlink. FUSE itself will take care of tracking symbolic links in paths, 
    *  so your path-evaluation code doesn't need to worry about it.
    */
   FileSystemResponse ReadSymbolicLink(1:string path, 2:i32 maxSize);
  
   /*
   * mkdir(const char* path, mode_t mode)
   * Create a directory with the given name. The directory permissions are encoded in mode. 
   * See mkdir(2) for details. This function is needed for any reasonable read/write filesystem.
   */
   FileSystemResponse CreateDir(1:string path, 2:Permission permissions);
  
   /*
   * unlink(const char* path)
   * Remove (delete) the given file, symbolic link, hard link, or special node. 
   * Note that if you support hard links, unlink only deletes the data when the last hard link is removed. 
   * See unlink(2) for details.
   */
   FileSystemResponse DeleteFile(1:string path);
   /*
   * rmdir(const char* path) 
   * Remove the given directory. This should succeed only if the directory is empty (except for "." and ".."). 
   * See rmdir(2) for details.
   */
   FileSystemResponse DeleteDir(1:string path);

   /*
   * symlink(const char* to, const char* from)
   * Create a symbolic link named "from" which, when evaluated, will lead to "to". 
   * Not required if you don't support symbolic links. NOTE: Symbolic-link support requires only readlink and symlink.
   * FUSE itself will take care of tracking symbolic links in paths, so your path-evaluation code doesn't need to worry about it.
   */
   FileSystemResponse CreateSymbolicLink(1:string destination, 2:string source);
   
   /*
   * rename(const char* from, const char* to)
   * Rename the file, directory, or other object "from" to the target "to". 
   * Note that the source and target don't have to be in the same directory, 
   * so it may be necessary to move the source to an entirely new directory. See rename(2) for full details.
   */
   FileSystemResponse Rename(1:string source, 2:string destination);

   /*
   * link(const char* from, const char* to)
   * Create a hard link between "from" and "to". Hard links aren't required for a working filesystem, 
   * and many successful filesystems don't support them. If you do implement hard links, be aware that 
   * they have an effect on how unlink works. See link(2) for details.
   */
   FileSystemResponse CreateHardLink(1:string to, 2:string destination);
   
   /*
   * chmod(const char* path, mode_t mode) 
   * Change the mode (permissions) of the given object to the given new permissions. 
   * Only the permissions bits of mode should be examined. See chmod(2) for details.
   */
   FileSystemResponse SetPermission(1:string path, 2:Permission permissions, 3: FileInfo info); 
   
   /*
   * chown(const char* path, uid_t uid, gid_t gid
   * Change the given object's owner and group to the provided values. 
   * See chown(2) for details. NOTE: FUSE doesn't deal particularly well with file ownership, 
   * since it usually runs as an unprivileged user and this call is restricted to the superuser. 
   * It's often easier to pretend that all files are owned by the user who mounted the filesystem, 
   * and to skip implementing this function.
   */
   FileSystemResponse SetOwner(1:string path, 2:OwenershipInfo owener, 3: FileInfo info);
   
   /*
   * truncate(const char* path, off_t size)
   * Truncate or extend the given file so that it is precisely size bytes long. See truncate(2) for details. 
   * This call is required for read/write filesystems, because recreating a file will first truncate it.
   */
   FileSystemResponse TruncateFile(1:string path, 2:i64 offset, 3: FileInfo info);
   
   /*
   * open(const char* path, struct fuse_file_info* fi)
   * Open a file. If you aren't using file handles, this function should just check for existence and permissions and 
   * return either success or an error code. If you use file handles, you should also allocate any necessary structures and set fi->fh. 
   * In addition, fi has some other fields that an advanced filesystem might find useful; see the structure definition in fuse_common.h 
   * for very brief commentary.
   */
   FileSystemResponse OpenFile(1:string path, 3: FileInfo info);
   
   /*
   * read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi)
   * Read sizebytes from the given file into the buffer buf, beginning offset bytes into the file. See read(2) for full details. Returns the number of bytes transferred, or 0 if offset was at or beyond the end of the file. Required for any sensible filesystem.
   */
   FileSystemResponse ReadFile(1:string path, 2:i32 size, 3:i64 offset, 4: FileInfo info);
   
   /*
   * write(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi)
   * As for read above, except that it can't return 0.
   */
   FileSystemResponse WriteFile(1:string path, 2:i64 offset, 3:binary buffer, 4: FileInfo info);
   
   /*
   * statfs(const char* path, struct statvfs* stbuf
   * Return statistics about the filesystem. See statvfs(2) for a description of the structure contents. Usually, you can ignore the path. Not required, but handy for read/write filesystems since this is how programs like df determine the free space.
   */
   FileSystemResponse GetFileSystemStats(1:string path);
   
   /*
   * release(const char* path, struct fuse_file_info *fi)
   * This is the only FUSE function that doesn't have a directly corresponding system call, although close(2) is related. Release is called when FUSE is completely done with a file; at that point, you can free up any temporarily allocated data structures. The IBM document claims that there is exactly one release per open, but I don't know if that is true.
   */
   FileSystemResponse ReleaseFile(1:string path, 2: FileInfo info);

   
   /*
   * fsync(const char* path, int isdatasync, struct fuse_file_info* fi)
   * Flush any dirty information about the file to disk. If isdatasync is nonzero, only data, not metadata, needs to be flushed. 
   * When this call returns, all file data should be on stable storage. Many filesystems leave this call unimplemented, 
   * although technically that's a Bad Thing since it risks losing data. If you store your filesystem inside a plain file on another filesystem, 
   * you can implement this by calling fsync(2) on that file, which will flush too much data (slowing performance) but achieve the desired guarantee.
   */
   FileSystemResponse FlushFileData(1:string path, 2: FileInfo info);



   /*
   * setxattr(const char* path, const char* name, const char* value, size_t size, int flags)
   * Set an extended attribute. See setxattr(2). This should be implemented only if HAVE_SETXATTR is true.
   */
   FileSystemResponse SetXAttribute(1:string path,2:KVList attributes, 3:i16 size,4:i32 flags);
   
   /*
   * getxattr(const char* path, const char* name, char* value, size_t size)
   * Read an extended attribute. See getxattr(2). This should be implemented only if HAVE_SETXATTR is true.
   */
   
   FileSystemResponse GetXAttribute(1:string path);
   
   /*
   * listxattr(const char* path, const char* list, size_t size)
   * List the names of all extended attributes. See listxattr(2). This should be implemented only if HAVE_SETXATTR is true.
   */
   
   FileSystemResponse ListXAttributes(1:string path);
    
   /*
   * int(* fuse_operations::removexattr)(const char *, const char *)
   * Remove extended attributes
   */
   FileSystemResponse RemoveXAttribute(1:string path,2:string attributeKey);
 
    /*
    * opendir(const char* path, struct fuse_file_info* fi)
    * Open a directory for reading.
    */
   FileSystemResponse OpenDirectory(1:string path, 2: FileInfo info);
 
   /*
   * readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi)
   * Return one or more directory entries (struct dirent) to the caller. This is one of the most complex FUSE functions. 
   * It is related to, but not identical to, the readdir(2) and getdents(2) system calls, and the readdir(3) library function. 
   * Because of its complexity, it is described separately below. Required for essentially any filesystem, 
   * since it's what makes ls and a whole bunch of other things work.
   */
   FileSystemResponse ReadDirectory(1:string path, 2: FileInfo info);

   /*
    * releasedir(const char* path, struct fuse_file_info *fi)
    * This is like release, except for directories.
    */
   FileSystemResponse ReleaseDir(1:string path, 2: FileInfo info);
   
   /*
   * fsyncdir(const char* path, int isdatasync, struct fuse_file_info* fi)
   * Like fsync, but for directories.
   */
   FileSystemResponse FlushDirectory(1:string path);

    /*
    * void* init(struct fuse_conn_info *conn)
    * Initialize the filesystem. This function can often be left unimplemented, but it can be a handy way to perform one-time setup such as allocating variable-sized data structures or initializing a new filesystem. The fuse_conn_info structure gives information about what features are supported by FUSE, and can be used to request certain capabilities (see below for more information). The return value of this function is available to all file operations in the private_data field of fuse_context. It is also passed as a parameter to the destroy() method.
    */
    FileSystemResponse InitFileSystem();
   
    /*
    * void destroy(void* private_data)
    * Called when the filesystem exits. The private_data comes from the return value of init.
    */
    void DestroyFileSystem(1:i16 fsPrivateId);
   
    /*
    * access(const char* path, mask)
    * This is the same as the access(2) system call. It returns -ENOENT if the path doesn't exist, -EACCESS 
    * if the requested permission isn't available, or 0 for success. Note that it can be called on files, directories, 
    * or any other object that appears in the filesystem. This call is not required but is highly recommended.
    */
    FileSystemResponse IsAccesible(1:string path,2:i32 accessMask)

   /*
   * Create and open a file If the file does not exist, first create it with the specified mode, and then open it.
   * If this method is not implemented or under Linux kernel versions earlier than 2.6.15, 
   * the mknod() and open() methods will be called instead.
   */
    FileSystemResponse Crate(1:string path,2:Permission permissions)

   /*
   * lock(const char* path, struct fuse_file_info* fi, int cmd, struct flock* locks)
   * Perform POSIX file locking operation The cmd argument will be either F_GETLK, F_SETLK or F_SETLKW.
   * For the meaning of fields in 'struct flock' see the man page for fcntl(2). The l_whence field will always be set to SEEK_SET.
   * For checking lock ownership, the 'fuse_file_info->owner' argument must be used.
   * For F_GETLK operation, the library will first check currently held locks, and 
   * if a conflicting lock is found it will return information without calling this method. 
   * This ensures, that for local locks the l_pid field is correctly filled in. 
   * The results may not be accurate in case of race conditions and in the presence of hard links, 
   * but it's unlikely that an application would rely on accurate GETLK results in these cases. 
   * If a conflicting lock is not found, this method will be called, and the filesystem may filsl out l_pid by a meaningful value,
   * or it may leave this field zero.
   * For F_SETLK and F_SETLKW the l_pid field will be set to the pid of the process performing the locking operation.
   * Note: if this method is not implemented, the kernel will still allow file locking to work locally. 
   * Hence it is only interesting for network filesystems and similar.
   */

   FileSystemResponse Lock(1:string path, 2:i32 cmd, 3:FileLock flock)
    
   /*
   * utimens(const char* path, const struct timespec ts[2]
   * Update the last access time of the given object from ts[0] and the last modification time from ts[1]. 
   * Both time specifications are given to nanosecond resolution, but your filesystem doesn't have to be that precise; 
   * see utimensat(2) for full details. Note that the time specifications are allowed to have certain special values;
   * however, I don't know if FUSE functions have to support them. 
   * This function isn't necessary but is nice to have in a fully functional filesystem.
   */
   FileSystemResponse UpdateTimes(1:string path, 2:i32 cmd, 3:FileTimeSpec timeSpec, 4: FileInfo info)

   /*
   * bmap(const char* path, size_t blocksize, uint64_t* blockno) 
   * This function is similar to bmap(9). If the filesystem is backed by a block device, 
   * it converts blockno from a file-relative block number to a device-relative block. 
   * It isn't entirely clear how the blocksize parameter is intended to be used.
   */

   FileSystemResponse MapBlock(1:string path, 2:i64 blocksize, 3:i64 blockIndex);

   /*
   * ===== TODO: To Keep it miniminalistinc filter out these if  no needed
   *
   * int (*write_buf) (const char *, struct fuse_bufvec *buf, off_t off,
   *                  struct fuse_file_info *);
   * int (*read_buf) (const char *, struct fuse_bufvec **bufp,
   *                 size_t size, off_t off, struct fuse_file_info *);
   * int (*flock) (const char *, struct fuse_file_info *, int op);
   *
   * int (*fallocate) (const char *, int, off_t, off_t,
   *                  struct fuse_file_info *);
   *
   * ssize_t (*copy_file_range) (const char *path_in,
   *                            struct fuse_file_info *fi_in,
   *                            off_t offset_in, const char *path_out,
   *                            struct fuse_file_info *fi_out,
   *                            off_t offset_out, size_t size, int flags);
   * off_t (*lseek) (const char *, off_t off, int whence, struct fuse_file_info *);
   *
   * ===== Not Planning to Support/Optinal Support.
   *
   * ioctl(const char* path, int cmd, void* arg, struct fuse_file_info* fi, unsigned int flags, void* data
   * Support the ioctl(2) system call. As such, almost everything is up to the filesystem. On a 64-bit machine, FUSE_IOCTL_COMPAT will be set for 32-bit ioctls. The size and direction of data is determined by _IOC_*() decoding of cmd. For _IOC_NONE, data will be NULL; for _IOC_WRITE data is being written by the user; for _IOC_READ it is being read, and if both are set the data is bidirectional. In all non-NULL cases, the area is _IOC_SIZE(cmd) bytes in size.
   * poll(const char* path, struct fuse_file_info* fi, struct fuse_pollhandle* ph, unsigned* reventsp);
   * Poll for I/O readiness. If ph is non-NULL, when the filesystem is ready for I/O it should call fuse_notify_poll (possibly asynchronously) with the specified ph; this will clear all pending polls. The callee is responsible for destroying ph with fuse_pollhandle_destroy() when ph is no longer needed.
   */
}