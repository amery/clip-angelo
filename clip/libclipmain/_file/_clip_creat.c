int
_clip_creat(ClipMachine * ClipMachineMemory, char *file, int flags, mode_t mode, int exclusive)
{
#ifndef OS_CYGWIN
   int fd, fl, lock;

   long hash = _clip_hashstr(file);

   int *fileopen;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   *err = 0;

   fl = flags & ~(O_TRUNC | O_CREAT | O_EXCL);
   lock = exclusive ? F_WRLCK : F_RDLCK;

   fd = open(file, fl);

   if (fd >= 0)
   {
      fileopen = HashTable_fetch(ClipMachineMemory->fileopens, hash);
      if (fileopen)
      {
	 (*fileopen)++;
      }
      else
      {
	 fileopen = malloc(sizeof(int));
	 *fileopen = 1;
	 HashTable_store(ClipMachineMemory->fileopens, fileopen, hash);
      }
      if (_clip_setlock(ClipMachineMemory, hash, fd, 0x7fffffff, (exclusive ? CLIP_LOCK_WRITE : CLIP_LOCK_READ)))
      {
	 _clip_close(ClipMachineMemory, hash, fd);
	 errno = EAGAIN;	/*11; *//* resource unavailable */
	 return -1;
      }
#ifdef USE_NCPFS
      if (ncp_openmode(fd, exclusive))
      {
	 _clip_unlock(ClipMachineMemory, hash, fd, 0x7fffffff, 0);
	 _clip_close(ClipMachineMemory, hash, fd);
	 return -1;
      }
#endif
      _clip_unlock(ClipMachineMemory, hash, fd, 0x7fffffff, 0);
      _clip_close(ClipMachineMemory, hash, fd);
      unlink(file);
   }

   /* creat call work buggy on network filesystems...
      druzus:
      and this is important for secure reason, hacker can
      create link form file to /any/other/important/file
      so we always should use pair of functions:
      unlink() and open(file, fl|O_EXCL, mode)
    */
   unlink(file);
   fd = open(file, fl | O_CREAT | O_EXCL, mode);
   if (fd != -1)
   {
      fileopen = HashTable_fetch(ClipMachineMemory->fileopens, hash);
      if (fileopen)
      {
	 (*fileopen)++;
      }
      else
      {
	 fileopen = malloc(sizeof(int));
	 *fileopen = 1;
	 HashTable_store(ClipMachineMemory->fileopens, fileopen, hash);
      }
   }

   if (fd != -1)
   {
      if (_clip_setlock(ClipMachineMemory, hash, fd, 0x7fffffff, (exclusive ? CLIP_LOCK_WRITE : CLIP_LOCK_READ)))
      {
	 _clip_unlock(ClipMachineMemory, hash, fd, 0x7fffffff, 0);
	 _clip_close(ClipMachineMemory, hash, fd);
	 return -1;
      }
#ifdef USE_NCPFS
      if (ncp_openmode(fd, exclusive))
      {
	 _clip_unlock(ClipMachineMemory, hash, fd, 0x7fffffff, 0);
	 _clip_close(ClipMachineMemory, hash, fd);
	 return -1;
      }
#endif
      errno = 0;
   }
   return fd;
#else
   HANDLE h;

   char *wp = malloc(PATH_MAX);

   int fd = -1;

   cygwin_conv_to_full_win32_path(file, wp);
   h = CreateFile(wp, GENERIC_READ | GENERIC_WRITE, exclusive ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   setmode((int) h, O_BINARY);
   if (h == INVALID_HANDLE_VALUE)
   {
      if (GetLastError())
      {
	 free(wp);
	 return -1;
      }
   }
   fd = cygwin_attach_handle_to_fd(wp, -1, h, 1, 0);
   free(wp);
   return fd;
#endif
}
