static void
stop_scan_mode(int fd)
{
   switch (scr_scan_mode)
   {
   case ScanNone:
      break;
   case ScanTerminal:
      if (scan_stop)
	 write(fd, scan_stop, strlen(scan_stop));
      break;
   case ScanIoctl:
      set_rawmode(fd, 0);
      break;
   }
}
