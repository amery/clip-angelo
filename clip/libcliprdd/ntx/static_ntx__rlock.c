static int
ntx__rlock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   if (ro->index->rd->shared)
   {
      struct flock fl;

      fl.l_type = F_RDLCK;
      fl.l_whence = SEEK_SET;
      fl.l_start = 1000000000;
      fl.l_len = 1;

      if (fcntl(ro->index->file.fd, F_SETLKW, &fl))
	 return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_readlock);
      ro->wlocked = 0;
   }
   return 0;
}
