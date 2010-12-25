int
restore_tty(ScreenBase * base)
{
   ScreenData *dp;

#ifdef _WIN32
   if (w32_console)
      return 0;
#endif
   if (scr_scan_mode)
      stop_scan_mode(0);

   if (!base)
      return 0;

   dp = (ScreenData *) base->data;
   return tcsetattr(base->fd, TCSADRAIN, &dp->start_mode);
}
