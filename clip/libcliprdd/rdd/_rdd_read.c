int
_rdd_read(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__)
{

#ifdef HAVE_MMAN_H
   struct stat st;

   int realen;
#endif
   if (file->dbf && !file->rlocked && !file->wlocked)
   {
      char buf[256];

      time_t tt;

      struct tm *tp;

      tt = time(0);
      tp = localtime(&tt);
      snprintf(buf, sizeof(buf), "%02d:%02d:%02d: Warning: unsafe DBF read: wlocked=%d; %s(%s)\n", tp->tm_hour, tp->tm_min, tp->tm_sec, file->wlocked, __PROC__, file->dbf);
      _clip_out_log(buf, strlen(buf));
      _clip_flush_log();
   }
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
   {
      if (pos + len > file->mapsize)
      {
	 if (fstat(file->fd, &st) == -1)
	    goto err;
	 if (file->mapsize < st.st_size)
	 {
	    if (munmap(file->md, file->mapsize) == -1)
	       goto err;
	    file->mapsize = st.st_size;
	    file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
	    if (file->md == (caddr_t) - 1)
	    {
	       if (lseek(file->fd, pos, SEEK_SET) == -1)
		  goto err;
	       if (read(file->fd, buf, len) == -1)
		  goto err;
	       return 0;
	    }
	 }
      }
      realen = (pos + len > file->mapsize) ? file->mapsize - pos : len;
      realen = realen < 0 ? 0 : realen;
      memcpy(buf, file->md + pos, realen);
      memset(buf + realen, 0, len - realen);
   }
   else
   {
#endif
      if (lseek(file->fd, pos, SEEK_SET) == -1)
	 goto err;
      if (read(file->fd, buf, len) == -1)
	 goto err;
#ifdef HAVE_MMAN_H
   }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_READ, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}
