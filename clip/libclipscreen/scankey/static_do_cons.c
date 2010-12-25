static void
do_cons(unsigned char value, char up_flag)
{
   if (up_flag)
      return;
   set_console(value, 0);
}
