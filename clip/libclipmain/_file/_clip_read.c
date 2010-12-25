int
_clip_read(C_FILE * cf, void *buf, size_t count)
{
   int n = 0, i = 0;

   errno = 0;
   if (count > 0)
   {
      fd_set rfds;

      struct timeval tv, *ptv = NULL;

      if (cf->timeout_of_C_FILE >= 0)
      {
	 tv.tv_sec = cf->timeout_of_C_FILE / 1000;
	 tv.tv_usec = (cf->timeout_of_C_FILE % 1000) * 1000;
	 ptv = &tv;
      }

      do
      {
	 if (ptv == NULL)
	    i = 1;
	 else
	 {
	    FD_ZERO(&rfds);
	    FD_SET(cf->fileno_of_C_FILE, &rfds);

	    i = _clip_select(cf->fileno_of_C_FILE + 1, &rfds, NULL, NULL, ptv);

	    if (i == 0 && (cf->type_of_C_FILE == FT_SOCKET || cf->type_of_C_FILE == FT_PIPE))
	    {
	       errno = EAGAIN;
	       i = -1;
	    }
	 }
	 if (i > 0)
	 {
	    i = read(cf->fileno_of_C_FILE, buf + n, count - n);
	    if (i > 0)
	    {
	       n += i;
#ifndef FILEIO_FULLREAD
	       break;
#endif
	    }
	    else if (i == 0 && (cf->type_of_C_FILE == FT_SOCKET || cf->type_of_C_FILE == FT_PIPE))
	    {			/* if other side close a connection
				   (pipe or socket) select allow to
				   read but read() returns 0. It's
				   POSIX systems behavior, Windows
				   (WinSock) does not respect it.
				 */
	       errno = EPIPE;
	       i = -1;
	    }
	 }
	 if (i == -1 && errno == EINTR)
	    i = 1;		/* operation interrupted by signal,
				   Let's try again, _clip_select
				   should decrease tv to avoid
				   infinite loop. */
      }
      while (i != -1 && i != 0 && n < count);

      if (i == -1 && errno == EPIPE)
	 cf->stat_of_C_FILE |= FS_EOF;
   }
   return n > 0 ? n : i;	/* POSIX allow to return -1 when some bytes
				   was read/written and then appear
				   error/interrupts(unblocked signal)
				   (it's undefined) but such behavior is
				   rather seldom and it complicates high level
				   code, so let's inform programmer about
				   bytes read/written even the whole operation
				   wasn't completed */
}
