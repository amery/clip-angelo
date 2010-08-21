void
_clip_recodeString(char *str, int len, int sourceCharset, int targetCharset)
{
   int i;

   unsigned char *s;

   if (sourceCharset == targetCharset)
      return;
   for (i = 0, s = (unsigned char *) str; i < len; ++i, ++s)
   {
      int ch = *s;

      if (ch < 128)
	 continue;
      switch (sourceCharset)
      {
      case CS_KOI:
	 break;
      case CS_ALT:
	 ch = alt2koi[ch - 128];
	 break;
      case CS_WIN:
	 ch = win2koi[ch - 128];
	 break;
      }
      if (ch < 128)
      {
	 *s = ch;
	 continue;
      }
      switch (targetCharset)
      {
      case CS_KOI:
	 break;
      case CS_ALT:
	 ch = koi2alt[ch - 128];
	 break;
      case CS_WIN:
	 ch = koi2win[ch - 128];
	 break;
      }
      *s = ch;
   }
}
