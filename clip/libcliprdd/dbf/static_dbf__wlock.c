static int
dbf__wlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   if (rd->shared)
   {
      if (!rd->locked)
      {
	 if (_clip_setlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, rd->lockstyle ? 0x10000000 : 1000000000, CLIP_LOCK_WRITE | CLIP_LOCK_LOWLEVEL | CLIP_LOCK_WAIT))

	    return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_writelock);
      }
      rd->locked++;
   }
   rd->file.wlocked = 1;
   return 0;
}
