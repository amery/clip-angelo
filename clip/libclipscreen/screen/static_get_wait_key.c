static unsigned long
get_wait_key(ScreenBase * base, long milliseconds, int raw)
{
   unsigned char ch;

   struct timeval end, tv, dt, etv;

   unsigned long key;

   gettimeofday(&tv, 0);
   dt.tv_sec = milliseconds / 1000;
   dt.tv_usec = (milliseconds % 1000) * 1000;

   etv.tv_sec = esc_delay_Screen / 1000;
   etv.tv_usec = (esc_delay_Screen % 1000) * 1000;

   timer_add(&tv, &dt, &end);

   /*newMatch_Key(base); */
   if (raw && !scr_scan_mode)
      return 0;
   if (!raw && scr_scan_mode)
   {
      key = scan_check();
#ifdef DBG
      printf("get_wait_key: %ld\r\n", key);
#endif
      if (key)
      {
	 scan_reset();
	 return key;
      }
   }

#ifdef _WIN32
   if (w32_console)
   {
      struct timeval timeout;

      long ms_timeout;

      for (;;)
      {
	 int r;

	 if (!raw)
	 {
	    while (w32_scan_buf_len)
	    {
	       ch = w32_readch();
	       if (match_Key(base, ch, &key))
	       {
		  newMatch_Key(base);
		  return key;
	       }
	    }
	 }

	 if (!milliseconds || timercmp(&end, &tv, <))
	 {
	    timeout.tv_sec = 0;
	    timeout.tv_usec = 0;
	 }
	 else
	 {
	    timer_sub(&end, &tv, &timeout);
	 }

	 ms_timeout = timeout.tv_sec * 1000 + timeout.tv_usec / 1000;

#ifdef USE_TASKS
	 r = Task_wait_read(0, ms_timeout);
	 if (r)
	    return 0;

#else
	 r = WaitForMultipleObjects(1, &w32_hStdIn, 1, ms_timeout);

	 if (r != WAIT_OBJECT_0)
	    return 0;

#endif

	 ch = w32_readch();
	 if (raw && scr_scan_mode)
	 {
	    /*if (!scan_push(ch))
	       goto again1; */
	    scan_push(ch);
#ifdef DBG
	    printf("get_wait_key return %d\r\n", ch);
#endif
	    return ch;
	 }
	 if (match_Key(base, ch, &key))
	 {
	    newMatch_Key(base);
	    return key;
	 }
	 /* again1: */
	 gettimeofday(&tv, 0);

      }
      return 0;
   }
#endif

   for (;;)
   {
      struct timeval timeout;

      int r, n;

      fd_set rfs;

      int esc_tv = 0;

      if (!milliseconds || timercmp(&end, &tv, <))
      {
	 timeout.tv_sec = 0;
	 timeout.tv_usec = 0;
      }
      else
      {
	 timer_sub(&end, &tv, &timeout);
      }

      if (timercmp(&etv, &timeout, <))
      {
	 timeout = etv;
	 esc_tv = 1;
      }

      FD_ZERO(&rfs);
      FD_SET(base->fd, &rfs);

      n = base->fd;
#ifdef HAVE_GPM_H
      if (gpm_fd >= 0)
      {
	 FD_SET(gpm_fd, &rfs);
	 if (gpm_fd > n)
	    n = gpm_fd;
      }
#endif

      r = task_select(n + 1, &rfs, 0, 0, &timeout);

      if (r == 0 && esc_tv && has_esc)
      {
	 newMatch_Key(base);
	 return 27;
      }

      if (r < 1)
      {
	 if (!milliseconds)
	    return 0;
	 else
	 {
	    gettimeofday(&tv, 0);
	    if (timercmp(&tv, &end, >))
	       return 0;
	    continue;
	 }
      }

#ifdef HAVE_GPM_H
      if (gpm_fd >= 0)
      {
	 if (FD_ISSET(gpm_fd, &rfs))
	 {
	    key = get_gpm_mouse(base);
	    if (key)
	       return key;
	 }
      }
#endif

      if (!FD_ISSET(base->fd, &rfs))
	 continue;

      if (read(base->fd, &ch, 1) < 1)
	 return 0;

      if (raw && scr_scan_mode)
      {
	 if (!scan_push(ch))
	    goto again;
	 return ch;
      }

      if (match_Key(base, ch, &key))
      {
	 newMatch_Key(base);
	 return key;
      }
    again:
      gettimeofday(&tv, 0);
   }
}
