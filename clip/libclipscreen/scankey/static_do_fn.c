static void
do_fn(unsigned char value, char up_flag)
{
#ifdef DBG2
   printf("do_fn: %d\r\n", value);
#endif

   //if (((int)value) < SIZE(func_table))
   if (((int) value) < (sizeof(func_table) / sizeof((func_table)[0])))
   {
#if 1
      if (K(KT_FN, value) == K_INSERT)
	 insert_state = !up_flag;

      if (up_flag)
	 return;

      switch (K(KT_FN, value))
      {
      case K_PGDN:
	 put_acqueue(KEY_PGDN);
	 break;
      case K_PGUP:
	 put_acqueue(KEY_PGUP);
	 break;
      case K_INSERT:
	 put_acqueue(KEY_INS);
	 break;
      case K_REMOVE:
	 put_acqueue(KEY_DEL);
	 break;
      case K_FIND:
	 put_acqueue(KEY_HOME);
	 break;
      case K_SELECT:
	 put_acqueue(KEY_END);
	 break;
      case K_F1:
	 put_fqueue(KEY_F1);
	 break;
      case K_F2:
	 put_fqueue(KEY_F2);
	 break;
      case K_F3:
	 put_fqueue(KEY_F3);
	 break;
      case K_F4:
	 put_fqueue(KEY_F4);
	 break;
      case K_F5:
	 put_fqueue(KEY_F5);
	 break;
      case K_F6:
	 put_fqueue(KEY_F6);
	 break;
      case K_F7:
	 put_fqueue(KEY_F7);
	 break;
      case K_F8:
	 put_fqueue(KEY_F8);
	 break;
      case K_F9:
	 put_fqueue(KEY_F9);
	 break;
      case K_F10:
	 put_fqueue(KEY_F10);
	 break;
      case K_F11:
	 put_fqueue(KEY_F11);
	 break;
      case K_F12:
	 put_fqueue(KEY_F12);
	 break;
      }
#else
      if (func_table[value])
	 puts_queue(func_table[value]);
#endif
   }
}
