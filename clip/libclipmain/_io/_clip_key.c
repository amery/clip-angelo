CLIP_DLLEXPORT int
_clip_key(ClipMachine * ClipMachineMemory, int timeout_ms, int mask)
{
   unsigned long key;

   int ckey = 0;

	/*_clip_fullscreen(ClipMachineMemory); */
	/*_clip_logg(0,"bbb1 clip_key,%p,%p",*ClipMachineMemory->kbdptr, ClipMachineMemory->kbdbuf);*/
   if (*ClipMachineMemory->kbdptr != ClipMachineMemory->kbdbuf)
   {
		/*_clip_logg(0,"bbb2 clear lastkey,%p,%p",*ClipMachineMemory->kbdptr, ClipMachineMemory->kbdbuf);*/
      (*ClipMachineMemory->kbdptr)--;
      ckey = ClipMachineMemory->lastkey = **ClipMachineMemory->kbdptr;
      return ckey;
   }

   signal(SIGINT, _clip_sigint_real);
   if (timeout_ms < 0)
      timeout_ms = 1000 * 6000;

   do
   {
      struct timeval end, tv, dt;

      gettimeofday(&tv, 0);
      dt.tv_sec = timeout_ms / 1000;
      dt.tv_usec = (timeout_ms % 1000) * 1000;

      timer_add(&tv, &dt, &end);

      key = getWait_Key(ClipMachineMemory->screen->base, timeout_ms);

      ckey = key2clip(key, mask);

      if (ckey != 0)
      {
	 if (timeout_ms != 0)
	    ClipMachineMemory->lastkey = ckey;

	 break;
      }

      gettimeofday(&tv, 0);
      timer_sub(&end, &tv, &dt);

      timeout_ms = dt.tv_sec * 1000 + dt.tv_usec / 1000;

   }
   while (timeout_ms > 0);

   signal(SIGINT, _clip_sigint);
   return ckey;
}
