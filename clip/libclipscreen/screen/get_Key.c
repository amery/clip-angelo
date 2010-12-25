unsigned long
get_Key(ScreenBase * base)
{
   unsigned char ch;

   unsigned long key;

   newMatch_Key(base);
   for (;;)
   {
#ifdef _WIN32
      if (w32_console)
	 ch = w32_readch();
      else
#endif
      {
	 struct timeval tv;

	 tv.tv_sec = esc_delay_Screen / 1000;
	 tv.tv_usec = (esc_delay_Screen % 1000) * 1000;
#ifdef HAVE_GPM_H
	 if (gpm_fd >= 0)
	 {
	    fd_set rfs;

	    int r, n;

	  again:
	    FD_ZERO(&rfs);
	    FD_SET(gpm_fd, &rfs);
	    FD_SET(base->fd, &rfs);
	    n = base->fd;
	    if (gpm_fd > n)
	       n = gpm_fd;

	    r = select(n + 1, &rfs, 0, 0, 0);
	    if (r > 0 && FD_ISSET(gpm_fd, &rfs))
	    {
	       key = get_gpm_mouse(base);
	       if (key)
		  return key;
	    }
	    if (!FD_ISSET(base->fd, &rfs))
	    {
	       if (r == 0 && has_esc)
	       {
		  newMatch_Key(base);
		  return 27;
	       }
	       goto again;
	    }
	 }
	 else
#endif
	 {
	    fd_set rfs;

	    int r, n;

	  again1:
	    FD_ZERO(&rfs);
	    FD_SET(base->fd, &rfs);
	    n = base->fd;

	    r = select(n + 1, &rfs, 0, 0, &tv);

	    if (r < 0)
	       return 0;

	    if (!FD_ISSET(base->fd, &rfs))
	    {
	       if (r == 0 && has_esc)
	       {
		  newMatch_Key(base);
		  return 27;
	       }

	       goto again1;
	    }
	 }

	 if (read(base->fd, &ch, 1) < 1)
	    return 0;
      }
      if (match_Key(base, ch, &key))
	 return key;
   }
}
