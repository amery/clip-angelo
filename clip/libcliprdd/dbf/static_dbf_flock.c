static int
dbf_flock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__)
{
   int er;

   *r = 1;
   if (rd->shared)
   {
      int i;

      if (rd->flocked)
	 return 0;
      for (i = 0; i < rd->nlocks; i++)
	 if ((er = dbf_ulock(ClipMachineMemory, rd, rd->locks[i], 0, __PROC__)))
	    return er;

      if (_clip_setlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + 1, CLIP_LOCK_WRITE | CLIP_LOCK_FLOCK | CLIP_LOCK_HILEVEL))

	 *r = 0;

      if (*r)
	 rd->flocked = 1;
   }
   return 0;
}
