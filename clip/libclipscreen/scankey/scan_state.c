unsigned int
scan_state(void)
{
   int r = shift_state;

   if (capslock_state)
      r |= (1 << SCAN_CAPSLOCK);
   if (scan_numlock_state)
      r |= (1 << SCAN_NUMLOCK);
   if (insert_state)
      r |= (1 << SCAN_INSERT);
   if (slockstate)
      r |= (1 << SCAN_SLOCK);
   if (lockstate)
      r |= (1 << SCAN_NATKBD);

#ifdef DBG
   printf("compute scan_state=%d\r\n", r);
#endif
   return r;
}
