int
scan_push(unsigned char scancode)
{
   unsigned char keycode;

   unsigned char raw_mode = 1;

   int down = !(scancode & 0x80);

   unsigned char up_flag = down ? 0 : 0200;

   unsigned short keysym;

   u_char type;

   int shift_final, shift_local = 0;

   unsigned short *key_map;

   if (!first_key)
   {
      first_key = 1;
      if (scancode & 0x80)
	 return 0;
   }

#ifdef DBG0
   printf("scan_push: %d\r\n", scancode);
#endif
   /*
    *  Convert scancode to keycode
    */
   if (!kbd_translate(scancode, &keycode))
      return 1;

#ifdef DBG0
   printf("translated: down=%d, keycode=%x\r\n", down, keycode);
#endif

   /*
    * At this point the variable `keycode' contains the keycode.
    * Note: the keycode must not be 0 (++Geert: on m68k 0 is valid).
    * We keep track of the up/down status of the key, and
    * return the keycode if in MEDIUMRAW mode.
    */

   if (up_flag)
   {
      if (!test_and_clear_bit(keycode, key_down))
      {
	 rep = 0;
	 up_flag = kbd_unexpected_up(keycode);
      }
   }
   else
      rep = test_and_set_bit(keycode, key_down);

#ifdef DBG1
   printf("up_flag=%d, rep=%d, shift_state=%d\r\n", up_flag, rep, shift_state);
#endif

   /*if (rep)
      return; */

   /* the XOR below used to be an OR */
   shift_final = shift_state ^ lockstate ^ slockstate;
   shift_local = (shift_final & (1 << KG_SHIFTL)) | (shift_final & (1 << KG_SHIFTR));
   key_map = key_maps[shift_final];
   if (shift_local == 0)
      shift_local = (1 << KG_SHIFT);
   if (key_map == NULL)
   {
      shift_final = shift_state ^ lockstate ^ slockstate;
      shift_final = shift_final ^ shift_local;
      shift_local = 0;
      key_map = key_maps[shift_final];
   }
   if (key_map != NULL)
   {
      keysym = key_map[keycode];
      type = KTYP(keysym);
#ifdef DBG2
      printf("\ntype=%x,shift_state=%d,shift_final=%d,keykode=%d,shift_local=%d\r\n", type, shift_state, shift_final, keycode, shift_local);
#endif

      if (type >= 0xf0)
      {
	 type -= 0xf0;
	 if (type == KT_LETTER)
	 {
	    type = KT_LATIN;
	    if (capslock_state)
	    {
	       shift_final = shift_final ^ shift_local;
	       key_map = key_maps[shift_final];
	       //key_map = key_maps[shift_final ^ (1 << KG_SHIFT)];
	       if (key_map)
		  keysym = key_map[keycode];
#ifdef DBG2
	       printf("  caps=%d,keykode=%d,keysym=%d,kg_shift=%d\r\n", shift_final, keycode, keysym, KG_SHIFT);
#endif
	    }
	 }
#ifdef DBG
	 printf("key_handler[%d] call: %s, keysym=0x%x, up_flag=0x%x\r\n", type, key_handler_names[type], keysym & 0xff, up_flag);
#endif
	 /*
	    printf("\nA keysym=%d,type=%d,%d",keysym,type,KT_SHIFT);
	  */
	 (*key_handler[type]) (keysym & 0xff, up_flag);
	 if (type != KT_SLOCK)
	    slockstate = 0;
      }
      else
      {
	 /* maybe only if (kbd->kbdmode == VC_UNICODE) ? */
	 if (!up_flag && !raw_mode)
	    to_utf8(keysym);
      }
   }
   else
   {
      /* maybe beep? */
      /* we have at least to update shift_state */
      key_map = key_maps[0];
      keysym = key_map[keycode];
      type = KTYP(keysym);
      /*
         printf("\nB keysym=%d,type=%d,%d",keysym,type,KT_SHIFT);
       */
      /*check CTRL,SHIFT,ALT keys status */
      if (keysym >= 63234 && keysym <= 63237)
	 (*key_handler[KT_SHIFT]) (keysym & 0xff, up_flag);
      else
	 compute_shiftstate();
   }
   return 1;
}
