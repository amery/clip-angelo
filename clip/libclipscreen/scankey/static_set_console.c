static void
set_console(int no, int diff)
{
   if (scr_scan_mode == ScanIoctl)
   {
#ifdef OS_MINGW
      char *tty = NULL;
#else
      char *tty = ttyname(0);
#endif
      int l;

      if (!tty)
	 return;
      l = strlen(tty);
      if (l < 8)
	 return;
      if (!isdigit(tty[8]))
	 return;
      if (diff)
	 no = atoi(tty + 8) - 1 + no;
#ifdef OS_LINUX
      ioctl(0, 0x5606 /*VT_ACTIVATE */ , no + 1);
#endif
   }
}
