static void
enter(void)
{
   if (diacr)
   {
      put_queue(diacr);
      diacr = 0;
   }
   put_queue(13);
#if 0
   if (vc_kbd_mode(kbd, VC_CRLF))
      put_queue(10);
#endif
}
