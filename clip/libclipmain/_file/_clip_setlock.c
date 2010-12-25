int
_clip_setlock(ClipMachine * ClipMachineMemory, long hash, int fd, off_t pos, int flags)
{
   int ok = 0;

   struct flock fl;

   fl.l_type = (flags & CLIP_LOCK_WRITE) ? F_WRLCK : F_RDLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = pos;
   fl.l_len = (flags & CLIP_LOCK_FLOCK) ? pos - 1 : 1;

   while (1)
   {
      if (fl.l_type == F_WRLCK)
	 ok = _clip_wrlock(ClipMachineMemory->tasklocks, hash, fd, &fl, !(flags & CLIP_LOCK_HILEVEL));
      else
	 ok = _clip_rdlock(ClipMachineMemory->tasklocks, hash, fd, &fl, !(flags & CLIP_LOCK_HILEVEL));

      if (ok || !(flags & CLIP_LOCK_WAIT))
	 break;
#ifdef USE_TASKS
      Task_sleep(1);
#else
      usleep(1);
#endif
   }
   return !ok;
}
