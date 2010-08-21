static void
do_slock(unsigned char value, char up_flag)
{
   //printf("\nA up_flag=%d,rep=%d,state=%ld",up_flag,rep,slockstate);
   if (up_flag || rep)
      return;
   if (value < BITS_PER_LONG)
      chg_bit(value, &slockstate);
   //chg_vc_kbd_slock(kbd, value);
   //printf("B up_flag=%d,rep=%d,state=%ld\n",up_flag,rep,slockstate);
}
