int
restart_tty(ScreenBase * base)
{
   struct termios ts;

   ScreenData *dp;

#ifdef _WIN32
   if (w32_console)
      return 0;
#endif
   if (scr_scan_mode)
      start_scan_mode(0);

   if (!base)
      return 0;

   dp = (ScreenData *) base->data;
   tcgetattr(base->fd, &ts);
   ts.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
   ts.c_oflag &= ~OPOST;
   ts.c_oflag |= ONLCR | OPOST;
   ts.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   ts.c_lflag |= ISIG;
   ts.c_cflag &= ~(CSIZE | PARENB);
   ts.c_cflag |= CS8;
   if (scr_scan_mode)
   {
      int i;

      for (i = 0; i < NCCS; i++)
	 ts.c_cc[i] = 0;
      ts.c_lflag &= ~ISIG;
      ts.c_cc[VINTR] = 0;
   }
   else
      ts.c_cc[VINTR] = 'C' - '@';

   ts.c_cc[VTIME] = 0;
   ts.c_cc[VMIN] = 1;

   if (!scr_scan_mode && dp->termcap_xo)
   {
      ts.c_cc[VSTOP] = dp->termcap_XF;
      ts.c_cc[VSTART] = dp->termcap_XN;
      ts.c_iflag |= IXON;
   }
   else
   {
      ts.c_iflag &= ~IXON;
   }

#ifdef VSUSP
   ts.c_cc[VSUSP] = 0;
#endif
#ifdef VDSUSP
   ts.c_cc[VDSUSP] = 0;
#endif
#ifdef VQUIT
   ts.c_cc[VQUIT] = 0;
#endif

   dp->work_mode = ts;

   return tcsetattr(base->fd, TCSANOW, &ts);
}
