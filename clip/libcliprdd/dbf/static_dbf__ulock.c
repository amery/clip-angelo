static int
dbf__ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   if (rd->shared)
   {
      if (rd->locked == 1)
      {
	 if (_clip_unlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, rd->lockstyle ? 0x10000000 : 1000000000, 0))

	    return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
      }
      rd->locked--;
   }
   rd->file.rlocked = 0;
   rd->file.wlocked = 0;
   return 0;
}
