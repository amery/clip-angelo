int
_clip_close(ClipMachine * ClipMachineMemory, long hash, int fd)
{
#ifndef OS_CYGWIN
   int *fileopen = HashTable_fetch(ClipMachineMemory->fileopens, hash);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   *err = 0;

   if (!fileopen)
      return -1;

   (*fileopen)--;
   if (!*fileopen)
   {
      TaskLock *tl = HashTable_fetch(ClipMachineMemory->tasklocks, hash);

      if (tl)
      {
	 if (tl->locks)
	    free(tl->locks);
	 HashTable_remove(ClipMachineMemory->tasklocks, hash);
	 free(tl);
      }
      HashTable_remove(ClipMachineMemory->fileopens, hash);
      free(fileopen);
      _clip_unlock(ClipMachineMemory, hash, fd, 0x7fffffff, 0);
   }
   return close(fd);
#else
   return close(fd);
#endif
}
