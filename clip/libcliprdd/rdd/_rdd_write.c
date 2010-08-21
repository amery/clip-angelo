int
_rdd_write(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__)
{
   struct stat st;

   if (file->dbf && !file->wlocked)
   {
      char buf[256];

      time_t tt;

      struct tm *tp;

      tt = time(0);
      tp = localtime(&tt);
      snprintf(buf, sizeof(buf), "%02d:%02d:%02d: Warning: unsafe DBF write: rlocked=%d; %s(%s)\n", tp->tm_hour, tp->tm_min, tp->tm_sec, file->rlocked, __PROC__, file->dbf);
      _clip_out_log(buf, strlen(buf));
      _clip_flush_log();
   }
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
   {
      if (pos + len > file->mapsize)
      {
	 if (munmap(file->md, file->mapsize) == -1)
	    goto err;
#ifdef _WIN32
	 {
	    void *bf;

	    int p;

	    if ((p = lseek(file->fd, 0, SEEK_END)) == -1)
	       goto err;
	    bf = calloc(1, pos + len - p);
	    if (write(file->fd, bf, pos + len - p) == -1)
	       goto err;
	    free(bf);
	 }
#else
	 if (lseek(file->fd, pos + len - 1, SEEK_SET) == -1)
	    goto err;
	 if (write(file->fd, "", 1) == -1)
	    goto err;
#endif
	 file->mapsize = pos + len;
	 file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
	 if (file->md == (caddr_t) - 1)
	 {
	    if (lseek(file->fd, pos, SEEK_SET) == -1)
	       goto err;
	    if (write(file->fd, buf, len) == -1)
	       goto err;
	    return 0;
	 }
      }
      memcpy(file->md + pos, buf, len);
   }
   else
   {
#endif
      /* ext2 fs issue */
      if (len == 0)
      {
	 if (fstat(file->fd, &st) == -1)
	    goto err;
	 if (pos > st.st_size)
	 {
	    if (lseek(file->fd, pos - 1, SEEK_SET) == -1)
	       goto err;
	    if (write(file->fd, "", 1) == -1)
	       goto err;
	 }
      }
      else
      {
	 if (lseek(file->fd, pos, SEEK_SET) == -1)
	    goto err;
	 if (write(file->fd, buf, len) == -1)
	    goto err;
      }
#ifdef HAVE_MMAN_H
   }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}
