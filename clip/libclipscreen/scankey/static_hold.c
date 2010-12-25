static void
hold(void)
{
   if (rep)
      return;

   /*
    * Note: SCROLLOCK will be set (cleared) by stop_tty (start_tty);
    * these routines are also activated by ^S/^Q.
    * (And SCROLLOCK can also be set by the ioctl KDSKBLED.)
    */
#if 0
   if (tty->stopped)
      start_tty(tty);
   else
      stop_tty(tty);
#endif
}
