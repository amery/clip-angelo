static void
do_meta(unsigned char value, char up_flag)
{
   if (up_flag)
      return;

#if 1
#ifdef DBG2
   printf("do_meta: %d\r\n", value);
#endif
   put_aqueue(value);
#else
   if ( /*vc_kbd_mode(kbd, VC_META) */ meta_state)
   {
      put_queue('\033');
      put_queue(value);
   }
   else
      put_queue(value | 0x80);
#endif
}
