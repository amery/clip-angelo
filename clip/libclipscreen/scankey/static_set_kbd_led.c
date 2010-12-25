static void
set_kbd_led(void)
{
#ifdef OS_LINUX
   int leds = 0;

   if (scan_numlock_state)
      leds |= LED_NUM;
   if (capslock_state)
      leds |= LED_CAP;
   if (slockstate)
      leds |= LED_SCR;

   ioctl(0, KDSETLED, leds);
#endif

}
