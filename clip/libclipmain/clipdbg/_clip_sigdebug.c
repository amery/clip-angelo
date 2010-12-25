void
_clip_sigdebug(int sig)
{
   sigset_t oldset, newset;

#ifndef OS_MINGW
   sigemptyset(&newset);
   sigaddset(&newset, SIGUSR1);
   sigprocmask(SIG_BLOCK, &newset, &oldset);
#endif

   _clip_logg(0, "got debug signal %d", sig);
   in_sigdebug = 1;
   if (!_clip_dbg_in)
   {
      char path[256];

      snprintf(path, sizeof(path), "/tmp/clip_dbg.%lu.in", (unsigned long) getpid());
      _clip_dbg_in = fopen(path, "rt");
      if (!_clip_dbg_in)
      {
	 _clip_logg(0, "debug: cannot open debug input '%s': %s", path, strerror(errno));
      }
      else
      {
	 setvbuf(_clip_dbg_in, 0, _IOLBF, 0);
	 /*fcntl(fileno(_clip_dbg_in), F_SETFL, O_NONBLOCK); */
	 dbg_in_fd = fileno(_clip_dbg_in);
      }
   }
   if (!_clip_dbg_out)
   {
      char path[256];

      snprintf(path, sizeof(path), "/tmp/clip_dbg.%lu.out", (unsigned long) getpid());
      _clip_dbg_out = fopen(path, "wt");
      if (!_clip_dbg_out)
      {
	 _clip_logg(0, "debug: cannot open debug output '%s': %s", path, strerror(errno));
      }
      else
      {
	 setvbuf(_clip_dbg_out, 0, _IOFBF, 0);
      }
   }

   if (!breakpoints._free)
   {
      init_Coll(&breakpoints, delete_ClipBreakPoint, 0);
      init_Coll(&sbreakpoints, 0, compare_ClipBreakPoint);
      init_Coll(&watchs, free, 0);
      init_Coll(&swatchs, 0, strcasecmp);
   }

   if (_clip_dbg_in && _clip_dbg_out)
   {
      debug_flag = 1;
      debugging = 1;
      _clip_debug(cur_ClipMachine());
   }
   in_sigdebug = 0;

#ifndef OS_MINGW
   sigprocmask(SIG_SETMASK, &oldset, 0);
#endif
}
