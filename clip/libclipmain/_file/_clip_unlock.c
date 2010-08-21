int
_clip_unlock(ClipMachine * ClipMachineMemory, long hash, int fd, off_t pos, int flags)
{
   int i;

   TaskLock *tl = HashTable_fetch(ClipMachineMemory->tasklocks, hash);

   OneTaskLock *lck;

   struct flock fl;

   fl.l_type = F_UNLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = pos;
   fl.l_len = (flags & CLIP_LOCK_FLOCK) ? pos - 1 : 1;

   if (flags & CLIP_LOCK_FLOCK)
   {
      if (tl && tl->flockfd != fd)
	 return 0;
      if (!fcntl(fd, F_SETLK, &fl))
      {
	 tl->flockfd = 0;
	 return 0;
      }
      return 1;
   }
   if (!tl)
      return 0;
   for (i = 0; i < tl->count; i++)
   {
      lck = tl->locks + i;
      if (lck->pos == fl.l_start)
      {
	 if (lck->fd == fd)
	 {
	    if (!fcntl(fd, F_SETLK, &fl))
	    {
	       memmove(tl->locks + i, tl->locks + i + 1, (tl->count - i - 1) * sizeof(OneTaskLock));
	       tl->count--;
	       tl->locks = realloc(tl->locks, tl->count * sizeof(OneTaskLock));
	       i--;
	    }
	 }
      }
   }
   return 0;
}
