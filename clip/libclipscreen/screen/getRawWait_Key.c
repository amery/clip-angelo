unsigned long
getRawWait_Key(ScreenBase * base, long milliseconds)
{
   if (!scr_scan_mode)
      return 0;
   return get_wait_key(base, milliseconds, 1);
}
