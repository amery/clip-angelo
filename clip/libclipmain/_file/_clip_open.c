int
_clip_open(ClipMachine * ClipMachineMemory, char *file, int flags, mode_t mode, int exclusive)
{
#ifndef OS_CYGWIN
   int fd, fl;

   long hash = _clip_hashstr(file);

   int *fileopen;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   *err = 0;

   if ((flags & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC))
      return _clip_creat(ClipMachineMemory, file, flags, mode, exclusive);

   /* 20031504 - fix for fopen(FO_TRUNC)
      fl = flags & ~(O_TRUNC|O_CREAT|O_EXCL);
    */
   fl = flags & ~(O_CREAT | O_EXCL);

   fd = open(file, fl, mode);

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
      if (_clip_setlock(ClipMachineMemory, hash, fd, 0x7fffffff, (exclusive ? CLIP_LOCK_WRITE : CLIP_LOCK_READ)))
      {
	 _clip_close(ClipMachineMemory, hash, fd);
	 err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
	 *err = 32;
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
   if (fd == -1)
      *err = 2;
   return fd;
#else
   HANDLE h;

   char *wp = malloc(PATH_MAX);

   int fd = -1;

   DWORD access = GENERIC_READ | GENERIC_WRITE;

   if (!(flags & (O_WRONLY | O_RDWR)))
      access &= ~GENERIC_WRITE;
   else if (flags & O_WRONLY)
      access &= ~GENERIC_READ;

   cygwin_conv_to_full_win32_path(file, wp);
   h = CreateFile(wp, access, exclusive ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
