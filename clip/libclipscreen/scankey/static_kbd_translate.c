static int
kbd_translate(unsigned char scancode, unsigned char *keycode)
{
   static int prev_scancode = 0;

   /* special prefix scancodes.. */
   if (scancode == 0xe0 || scancode == 0xe1)
   {
      prev_scancode = scancode;
#ifdef DBG0
      printf("kbd_translate1: return 0: scan=%d prev=%d\r\n", scancode, prev_scancode);
#endif

      return 0;
   }

   /* 0xFF is sent by a few keyboards, ignore it. 0x00 is error */
   if (scancode == 0x00 || scancode == 0xff)
   {
      prev_scancode = 0;

      return 0;
   }

   scancode &= 0x7f;

   if (prev_scancode)
   {
      /*
       * usually it will be 0xe0, but a Pause key generates
       * e1 1d 45 e1 9d c5 when pressed, and nothing when released
       */
      if (prev_scancode != 0xe0)
      {
	 if (prev_scancode == 0xe1 && scancode == 0x1d)
	 {
	    prev_scancode = 0x100;
	    return 0;
	 }
	 else if (prev_scancode == 0x100 && scancode == 0x45)
	 {
	    *keycode = E1_PAUSE;
	    prev_scancode = 0;
	 }
	 else
	 {
	    prev_scancode = 0;
	    return 0;
	 }
      }
      else
      {
	 prev_scancode = 0;
	 /*
	  *  The keyboard maintains its own internal caps lock and
	  *  num lock statuses. In caps lock mode E0 AA precedes make
	  *  code and E0 2A follows break code. In num lock mode,
	  *  E0 2A precedes make code and E0 AA follows break code.
	  *  We do our own book-keeping, so we will just ignore these.
	  */
	 /*
	  *  For my keyboard there is no caps lock mode, but there are
	  *  both Shift-L and Shift-R modes. The former mode generates
	  *  E0 2A / E0 AA pairs, the latter E0 B6 / E0 36 pairs.
	  *  So, we should also ignore the latter. - aeb@cwi.nl
	  */
	 if (scancode == 0x2a || scancode == 0x36)
	    return 0;

	 if (e0_keys[scancode])
	    *keycode = e0_keys[scancode];
	 else
	 {
	    return 0;
	 }
      }
   }
   else if (scancode >= SC_LIM)
   {
      /* This happens with the FOCUS 9000 keyboard
         Its keys PF1..PF12 are reported to generate
         55 73 77 78 79 7a 7b 7c 74 7e 6d 6f
         Moreover, unless repeated, they do not generate
         key-down events, so we have to zero up_flag below */
      /* Also, Japanese 86/106 keyboards are reported to
         generate 0x73 and 0x7d for \ - and \ | respectively. */
      /* Also, some Brazilian keyboard is reported to produce
         0x73 and 0x7e for \ ? and KP-dot, respectively. */

      *keycode = high_keys[scancode - SC_LIM];

      if (!*keycode)
      {
	 return 0;
      }
   }
   else
      *keycode = scancode;
   return 1;
}
