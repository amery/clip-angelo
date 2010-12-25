int
_clip_write(C_FILE * cf, void *buf, size_t count)
{
   int n = 0, i = 0;

   errno = 0;
   if (count > 0)
   {
      fd_set wfds;

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
	    FD_ZERO(&wfds);
	    FD_SET(cf->fileno_of_C_FILE, &wfds);

	    i = _clip_select(cf->fileno_of_C_FILE + 1, NULL, &wfds, NULL, ptv);

	    if (i == 0 && (cf->type_of_C_FILE == FT_SOCKET || cf->type_of_C_FILE == FT_PIPE))
	    {
	       errno = EAGAIN;
	       i = -1;
	    }
	 }
	 if (i > 0)
	 {
	    i = write(cf->fileno_of_C_FILE, buf + n, count - n);
	    if (i > 0)
	    {
	       n += i;
#ifndef FILEIO_FULLWRITE
	       break;
#endif
	    }
	 }
	 if (i == -1 && errno == EINTR)
	    i = 1;		/* operation interrupted by signal,
				   Let's try again, _clip_select
				   should decrease tv to avoid
				   infinite loop. */
      }
      while (i != -1 && i != 0 && n < count);
   }
   return n > 0 ? n : i;
}
