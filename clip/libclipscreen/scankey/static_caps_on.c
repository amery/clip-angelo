static void
caps_on(void)
{
   if (rep)
      return;
   /*set_vc_kbd_led(kbd, VC_CAPSLOCK); */
   capslock_state = 1;
   set_kbd_led();
}
