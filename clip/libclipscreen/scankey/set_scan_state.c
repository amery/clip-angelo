unsigned int
set_scan_state(int value, int locktype)
{
   int r = -1;

   switch (locktype)
   {
   case 1:
      r = scan_numlock_state;
      if (value >= 0)
	 scan_numlock_state = value;
      break;
   case 2:
      r = capslock_state;
      if (value >= 0)
	 capslock_state = value;
      break;
   case 3:
      r = slockstate;
      if (value >= 0)
	 slockstate = value;
      break;
   case 4:
      r = insert_state;
      if (value >= 0)
	 insert_state = value;
      break;
   }

#ifdef DBG
   printf("set scan_state=%d,%d\r\n", value, locktype);
#endif
   return r;
}
