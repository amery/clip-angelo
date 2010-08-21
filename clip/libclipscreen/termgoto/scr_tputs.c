int
scr_tputs(const char *str, int affcnt, int (*put_c) (int, void *), void *par)
{
   int delay = 0, c;

#if 1
   /* termcap style delay */
   delay = atoi(str);
   for (; (c = *str); ++str)
   {
      switch (c)
      {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
	 continue;
      }
      break;
   }

   while ((c = *str++))
   {
      switch (c)
      {
      case '$':
	 if (*str == '<')
	 {
	    ++str;
	    delay = atoi(str);
	    while ((c = *str++) != '>' && str)
	       ;
	    break;
	 }
      default:
	 put_c(c, par);
	 break;
      }
   }
#endif
   while (affcnt-- > 0)
      do_delay(delay);

   return 0;
}
