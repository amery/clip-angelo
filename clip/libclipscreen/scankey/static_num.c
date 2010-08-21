static void
num(void)
{
   if ( /*vc_kbd_mode(kbd,VC_APPLIC) */ applic_mode)
      applkey('P', 1);
   else
      bare_num();
}
