/*
 * Bind this to Shift-NumLock if you work in application keypad mode
 * but want to be able to change the NumLock flag.
 * Bind this to NumLock if you prefer that the NumLock key always
 * changes the NumLock flag.
 */
static void
bare_num(void)
{
   if (!rep)
   {
      /*chg_vc_kbd_led(kbd,VC_NUMLOCK); */
      scan_numlock_state = !scan_numlock_state;
      set_kbd_led();
   }
}
