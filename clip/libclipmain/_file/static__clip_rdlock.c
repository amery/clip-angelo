static int
_clip_rdlock(HashTable * locks, long hash, int fd, struct flock *fl, int lowlevel)
{
   int i;

   TaskLock *tl = HashTable_fetch(locks, hash);

   OneTaskLock *lck;

   if (tl && tl->flockfd && !lowlevel)
      return (fd == tl->flockfd);

   if (!tl)
   {
      if (!fcntl(fd, F_SETLK, fl))
      {
	 tl = calloc(1, sizeof(TaskLock));
	 tl->count = 1;
	 lck = tl->locks = calloc(1, sizeof(OneTaskLock));
	 lck->pos = fl->l_start;
	 lck->write = 0;
	 lck->fd = fd;
	 HashTable_store(locks, tl, hash);
	 return 1;
      }
      return 0;
   }
   for (i = 0; i < tl->count; i++)
   {
      lck = tl->locks + i;
      if (lck->pos == fl->l_start)
      {
	 if (lck->fd == fd)
	 {
	    if (!lck->write)
	    {
	       return 1;
	    }
	    else
	    {
	       if (!fcntl(fd, F_SETLK, fl))
	       {
		  lck->write = 0;
		  return 1;
	       }
	       return 0;
	    }
	 }
	 else
	 {
	    if (lck->write)
	       return 0;
	 }
      }
   }
   if (!fcntl(fd, F_SETLK, fl))
   {
      tl->count++;
      tl->locks = realloc(tl->locks, sizeof(OneTaskLock) * tl->count);
      lck = tl->locks + tl->count - 1;
      lck->pos = fl->l_start;
      lck->write = 0;
      lck->fd = fd;
      return 1;
   }
   return 0;
}
