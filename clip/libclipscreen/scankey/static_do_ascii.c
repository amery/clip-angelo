static void
do_ascii(unsigned char value, char up_flag)
{
   int base;

#ifdef DBG2
   printf("do_acii: %d\r\n", value);
#endif

   if (up_flag)
      return;

   if (value < 10)		/* decimal input of code, while Alt depressed */
      base = 10;
   else
   {				/* hexadecimal input of code, while AltGr depressed */
      value -= 10;
      base = 16;
   }

   if (npadch == -1)
      npadch = value;
   else
      npadch = npadch * base + value;
}
