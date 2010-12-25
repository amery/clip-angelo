unsigned long
getRaw_Key(ScreenBase * base)
{
#if !defined(OS_MINGW) && 1
   return getRawWait_Key(base, 1000 * 6000);
#else
   unsigned char ch = 0;

#ifdef _WIN32
   if (w32_console)
      ch = w32_readch();
   else
#endif
   if (!scr_scan_mode)
      return 0;

#ifdef _WIN32
   if (!w32_console)
#endif
      read(base->fd, &ch, 1);

   if (scr_scan_mode)
      scan_push(ch);
   else
   {
      unsigned long r = 0;

      if (term_match_Key(base, ch, &r))
	 return r;
      else
	 return 0;
   }

   return ch;
#endif
}
