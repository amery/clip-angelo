static int
dbf_rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   *r = 1;
   if (rd->shared)
   {
      int i;

      for (i = 0; i < rd->nlocks; i++)
      {
	 if (rd->locks[i] == rec)
	    return 0;
      }
      if (!rd->flocked)
      {
	 if (_clip_setlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, (rd->readonly ? CLIP_LOCK_READ : CLIP_LOCK_WRITE) | CLIP_LOCK_HILEVEL))

	    *r = 0;

	 if (*r)
	 {
	    rd->locks = realloc(rd->locks, (rd->nlocks + 1) * sizeof(unsigned int));
	    rd->locks[rd->nlocks] = rec;
	    rd->nlocks++;
	 }
      }
   }
   return 0;
}
