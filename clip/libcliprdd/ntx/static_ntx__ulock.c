static int
ntx__ulock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   int er = 0;

   if (ro->index->rd->shared)
   {
      struct flock fl;

      if (ro->wlocked)
      {
	 char cnt[2];

	 er = rdd_read(ClipMachineMemory, &ro->index->file, 2, 2, cnt, __PROC__);
	 ro->cnt = _rdd_ushort((unsigned char *) cnt) + 1;
	 _rdd_put_ushort((unsigned char *) cnt, ro->cnt);
	 er = rdd_write(ClipMachineMemory, &ro->index->file, 2, 2, cnt, __PROC__);
      }
      fl.l_type = F_UNLCK;
      fl.l_whence = SEEK_SET;
      fl.l_start = 1000000000;
      fl.l_len = 1;

      if (fcntl(ro->index->file.fd, F_SETLKW, &fl))
	 return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
      ro->wlocked = 0;
   }
   return er;
}
