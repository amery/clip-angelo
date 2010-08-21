static void
do_shift(unsigned char value, char up_flag)
{
   int old_state = shift_state;

#ifdef DBG2
   printf("do_shift: %d\r\n", value);
#endif

   if (rep && !up_flag)
      return;

   /* Mimic typewriter:
      a CapsShift key acts like Shift but undoes CapsLock */
   if (value == KVAL(K_CAPSSHIFT))
   {
      value = KVAL(K_SHIFT);
      if (!up_flag)
	 capslock_state = 0;
      set_kbd_led();
      /*clr_vc_kbd_led(kbd, VC_CAPSLOCK); */
   }

   if (up_flag)
   {
      /* handle the case that two shift or control
         keys are depressed simultaneously */
      if (k_down[value])
	 k_down[value]--;
   }
   else
      k_down[value]++;

   if (k_down[value])
      shift_state |= (1 << value);
   else
      shift_state &= ~(1 << value);

   /* kludge */
   if (up_flag && shift_state != old_state && npadch != -1)
   {
      put_queue(npadch & 0xff);
      npadch = -1;
   }
#ifdef DBG2
   printf("do shift=%ld\r\n", shift_state);
#endif
}
