static void
do_spec(unsigned char value, char up_flag)
{
#ifdef DBG2
   printf("do_spec: %d\r\n", value);
#endif

   if (up_flag)
      return;
   if (value >= SIZE(spec_fn_table))
      return;
#ifdef DBG
   printf("spec_fn call(%d): %s\r\n", value, spec_fn_names[value]);
#endif
   spec_fn_table[value] ();
}
