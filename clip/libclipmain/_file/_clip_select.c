/* TODO? For native Windows port (no CygWin) this function should
   use different mechanism for differ type of handles, select()
   is a WinSock function and operates only on sockets. */
int
_clip_select(int n, fd_set * rfds, fd_set * wfds, fd_set * efds, struct timeval *tv)
{
   int ret;

   if (tv == NULL || (tv->tv_sec == 0 && tv->tv_usec == 0))
      ret = select(n, rfds, wfds, efds, tv);
   else
   {
      /* Linux decrease timeval struct in timeout parameter to
         indicate how much time was left. Other system doesn't
         do that so we have consider timeout to be undefined and
         decrease it ourself. */
#if defined(HAVE_GETTIMEOFDAY) && !defined(OS_LINUX)
      struct timeval beg, end;

      gettimeofday(&beg, NULL);
      timer_add(&beg, tv, &end);
#endif

#ifdef USE_TASKS
      //ret = select(n, rfds, wfds, efds, tv);
      ret = task_select(n, rfds, wfds, efds, tv);
#else
      ret = select(n, rfds, wfds, efds, tv);
#endif

#ifndef OS_LINUX
      if ((ret == -1 && errno != EINTR) || ret == 0)
#else
      if (ret == -1 && errno != EINTR)
#endif
      {
	 /* Bad handle, parameters error or out of memory,
	    in any of such cases we shouldn't wait longer
	    (or select finished with no event) */
	 tv->tv_sec = tv->tv_usec = 0;
      }
#if defined(HAVE_GETTIMEOFDAY) && !defined(OS_LINUX)
      else
      {
	 gettimeofday(&beg, NULL);
	 timer_sub(&end, &beg, tv);
	 if (tv->tv_sec < 0)
	    tv->tv_sec = tv->tv_usec = 0;
      }
#endif
   }
   return ret;
}
