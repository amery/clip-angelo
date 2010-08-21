static int
t_select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval *timeout)
{
   int r = 0;

   long ms_timeout = 0;

   if (!readfds || !FD_ISSET(0, readfds))
      return select(nfds, readfds, writefds, exceptfds, timeout);

   if (w32_hStdIn == INVALID_HANDLE_VALUE)
      w32_hStdIn = GetStdHandle(STD_INPUT_HANDLE);

   if (w32_hStdIn == INVALID_HANDLE_VALUE)
      return select(nfds, readfds, writefds, exceptfds, timeout);

   if (timeout)
      ms_timeout = timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
   else
      ms_timeout = 1000 * 60 * 60 * 24;

   FD_CLR(0, readfds);

   if (zero_fds(readfds) && !writefds && !exceptfds)
   {
      r = WaitForMultipleObjects(1, &w32_hStdIn, 1, ms_timeout);
      if (r == WAIT_OBJECT_0)
      {
	 FD_SET(0, readfds);
	 return 1;
      }
      else
	 return 0;
   }

   while (ms_timeout > 0)
   {
      struct timeval tv;

      long wait;

      r = WaitForMultipleObjects(1, &w32_hStdIn, 1, 0);
      if (r == WAIT_OBJECT_0)
      {
	 tv.tv_sec = 0;
	 tv.tv_usec = 0;
	 r = select(nfds, readfds, writefds, exceptfds, &tv);
	 FD_SET(0, readfds);
	 if (r >= 0)
	    return r + 1;
	 return r;
      }

      if (ms_timeout > 100)
	 wait = 100;
      else
	 wait = ms_timeout;
      ms_timeout -= wait;

      tv.tv_sec = wait / 1000;
      tv.tv_usec = (wait % 1000) * 1000;

      r = select(nfds, readfds, writefds, exceptfds, &tv);

      if (r)
	 return r;
   }

   return 0;
}
