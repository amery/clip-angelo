static void
do_cur(unsigned char value, char up_flag)
{
#if 1
#ifdef DBG2
   printf("do_cur: %d\r\n", value);
#endif
   if (up_flag)
      return;

   switch (K(KT_CUR, value))
   {
   case K_DOWN:
      put_acqueue(KEY_DOWN);
      break;
   case K_LEFT:
      put_acqueue(KEY_LEFT);
      break;
   case K_RIGHT:
      put_acqueue(KEY_RIGHT);
      break;
   case K_UP:
      put_acqueue(KEY_UP);
      break;
   }
#else
   static const char *cur_chars = "BDCA";

   if (up_flag)
      return;

   applkey(cur_chars[value], /*vc_kbd_mode(kbd,VC_CKMODE) */ 0);
#endif
}
