static int
dbf_forlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   *r = _clip_setlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, CLIP_LOCK_WRITE | CLIP_LOCK_HILEVEL);
   if (!*r)
   {
      if (_clip_unlock(ClipMachineMemory, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, 0))

	 return rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
   }
   return 0;
}
