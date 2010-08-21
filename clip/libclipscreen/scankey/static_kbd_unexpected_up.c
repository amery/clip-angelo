static char
kbd_unexpected_up(unsigned char keycode)
{
   /* unexpected, but this can happen: maybe this was a key release for a
      FOCUS 9000 PF key; if we want to see it, we have to clear up_flag */
   if (keycode >= SC_LIM || keycode == 85)
      return 0;
   else
      return 0200;
}
