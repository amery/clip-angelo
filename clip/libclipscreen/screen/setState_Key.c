int
setState_Key(ScreenBase * base, int value, int locktype)
{
   int ret = 0;

   /* if (scr_scan_mode) */
   ret = set_scan_state(value, locktype);

   return ret;
}
