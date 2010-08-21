static void
caps_toggle(void)
{
   if (rep)
      return;
   /*chg_vc_kbd_led(kbd, VC_CAPSLOCK); */
   capslock_state = !capslock_state;
   set_kbd_led();
}
