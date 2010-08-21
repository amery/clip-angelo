static int
dbf_ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__)
{
   if (rd->shared)
   {
      int i;

      if (rec)
      {
	 for (i = 0; i < rd->nlocks; i++)
	 {
	    if (rd->locks[i] == rec)
	       break;
	 }
	 if (i == rd->nlocks)
	    return 0;
	 if (!rd->flocked)
	 {
	    if (_clip_unlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, 0))

	       return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
	 }
	 rd->nlocks--;
	 for (; i < rd->nlocks; i++)
	    rd->locks[i] = rd->locks[i + 1];
	 rd->locks = realloc(rd->locks, rd->nlocks * sizeof(unsigned int));
      }
      else
      {
	 if (rd->flocked)
	 {
	    if (flock)
	    {
	       if (_clip_unlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + 1, CLIP_LOCK_FLOCK))

		  return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);

	       rd->flocked = 0;
	       rd->locks = realloc(rd->locks, 0);
	       rd->nlocks = 0;
	    }
	 }
	 else
	 {
	    for (i = 0; i < rd->nlocks; i++)
	    {
	       if (_clip_unlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rd->locks[i], 0))

		  return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
	    }
	    rd->locks = realloc(rd->locks, 0);
	    rd->nlocks = 0;
	 }
      }
   }
   return 0;
}
