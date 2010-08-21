static int
w32_readch(void)
{
   INPUT_RECORD inputBuffer;

   DWORD dwInputEvents;

#ifdef DBG
   printf("w32_readch()\r\n");
#endif
   if (w32_scan_buf_len)
      return w32_get_scan_buf();

   ReadConsoleInput(w32_hStdIn, &inputBuffer, 1, &dwInputEvents);

   if (inputBuffer.EventType == KEY_EVENT)
   {
      if (inputBuffer.Event.KeyEvent.bKeyDown
	  && (inputBuffer.Event.KeyEvent.wVirtualScanCode == 27)
	  && (inputBuffer.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) && (inputBuffer.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)))
      {
	 printf("\nescape key pressed; exiting...\n");
	 exit(1);
      }
      w32_push_key_event(&inputBuffer.Event.KeyEvent);
   }
   else if (inputBuffer.EventType == MOUSE_EVENT)
   {
      w32_push_mouse_event(&inputBuffer.Event.MouseEvent);
   }

   return w32_get_scan_buf();
}
