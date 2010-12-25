static void
do_lock(unsigned char value, char up_flag)
{
   if (up_flag || rep)
      return;
   if (value < BITS_PER_LONG)
      chg_bit(value, &lockstate);
   /*chg_vc_kbd_lock(kbd, value); */
}
