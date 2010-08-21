int
getState_Key(ScreenBase * base)
{
   if (scr_scan_mode)
      return scan_state();
   else
   {
      ScreenData *dp = (ScreenData *) base->data;

      int m1 = dp->prev_has_meta1;

      int m2 = dp->prev_has_meta2;

      int r = 0;

      if (m1 && m2)
	 r |= (1 << KEY_SHIFT);
      else if (m1)
	 r |= (1 << KEY_ALT);
      else if (m2)
	 r |= (1 << KEY_CTRL);
      return r;
   }
}
