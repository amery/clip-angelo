static void
w32_push_key_event(KEY_EVENT_RECORD * kp)
{
   unsigned char ch;

   unsigned short word;

   int i;

   DWORD pstate;

   static int first = 1;

   word = kp->wVirtualScanCode;
   pstate = kp->dwControlKeyState;

   if (first && word != 0x45 && (pstate & NUMLOCK_ON))
   {
      first = 0;
      scan_numlock_state = 1;
   }

#ifdef DBG
   printf("\nread key: scan=0x%x pstate=0x%x count=%d down=%d\r\n", word, pstate, kp->wRepeatCount, kp->bKeyDown);
#endif
   for (i = 0; i < kp->wRepeatCount; i++)
   {
      if ((pstate & ENHANCED_KEY) && (word != 0x45))
      {
	 ch = 0xe0;
	 w32_put_scan_buf(ch);
      }
      ch = word /*& 0xff */ ;

      if (!kp->bKeyDown)
	 ch |= 0x80;
      w32_put_scan_buf(ch);
   }
}
