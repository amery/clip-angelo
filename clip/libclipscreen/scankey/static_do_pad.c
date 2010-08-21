static void
do_pad(unsigned char value, char up_flag)
{
   static const char pad_chars[] = "0123456789+-*/\015,.?()";

   /*static const char app_map[] = "pqrstuvwxylSRQMnnmPQ"; */

#ifdef DBG2
   printf("do_pad: %d\r\n", value);
#endif

   if (up_flag)
      return;			/* no action, if this is a key release */

#if 0
   /* kludge... shift forces cursor/number keys */
   if ( /*vc_kbd_mode(kbd,VC_APPLIC) */ applic_mode && !k_down[KG_SHIFT])
   {
      if (value < sizeof(app_map))
	 applkey(app_map[value], 1);
      return;
   }
#endif

   switch (value)
   {
   case KVAL(K_PPLUS):
      put_acqueue('+');
      return;
   case KVAL(K_PMINUS):
      put_acqueue('-');
      return;
   case KVAL(K_PSLASH):
      put_acqueue('/');
      return;
   case KVAL(K_PSTAR):
      put_acqueue('*');
      return;
   }

   if ((!scan_numlock_state	/*vc_kbd_led(kbd,VC_NUMLOCK) */
	&& !(shift_state & ((1 << KG_SHIFT) | (1 << KG_SHIFTL) | (1 << KG_SHIFTR)))) || (scan_numlock_state && (shift_state & ((1 << KG_CTRL) | (1 << KG_CTRLL) | (1 << KG_CTRLR)))))
   {
      switch (value)
      {
      case KVAL(K_PCOMMA):
      case KVAL(K_PDOT):
	 do_fn(KVAL(K_REMOVE), 0);
	 return;
      case KVAL(K_P0):
	 do_fn(KVAL(K_INSERT), 0);
	 return;
      case KVAL(K_P1):
	 do_fn(KVAL(K_SELECT), 0);
	 return;
      case KVAL(K_P2):
	 do_cur(KVAL(K_DOWN), 0);
	 return;
      case KVAL(K_P3):
	 do_fn(KVAL(K_PGDN), 0);
	 return;
      case KVAL(K_P4):
	 do_cur(KVAL(K_LEFT), 0);
	 return;
      case KVAL(K_P6):
	 do_cur(KVAL(K_RIGHT), 0);
	 return;
      case KVAL(K_P7):
	 do_fn(KVAL(K_FIND), 0);
	 return;
      case KVAL(K_P8):
	 do_cur(KVAL(K_UP), 0);
	 return;
      case KVAL(K_P9):
	 do_fn(KVAL(K_PGUP), 0);
	 return;
      case KVAL(K_P5):
	 applkey('G', /*vc_kbd_mode(kbd, VC_APPLIC) */ applic_mode);
	 return;
      }
   }

   if (value < sizeof(pad_chars))
      /*put_fqueue(pad_chars[value]); */
      put_queue(pad_chars[value]);
#if 0
   if (value == KVAL(K_PENTER) && vc_kbd_mode(kbd, VC_CRLF))
      put_queue(10);
#endif
}
