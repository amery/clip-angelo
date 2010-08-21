static void
start_scan_mode(int fd)
{
   switch (scr_scan_mode)
   {
   case ScanNone:
      break;
   case ScanTerminal:
      if (scan_start)
	 write(fd, scan_start, strlen(scan_start));
      break;
   case ScanIoctl:
      set_rawmode(fd, 1);
      break;
   }
}
