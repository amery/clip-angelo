CLIP_DLLEXPORT int
_clip_strnncmp(const char *str1, const char *str2, int len1, int len2)
{
   unsigned char ch1 = 0, ch2 = 0;

   const unsigned char *us1, *us2;

   for (us1 = (const unsigned char *) str1, us2 = (const unsigned char *) str2; len1 && len2; us1++, us2++, len1--, len2--)
   {
      ch1 = *(unsigned char *) us1;
      ch2 = *(unsigned char *) us2;
      if (ch1 < 32 && ch2 > 31)
	 ch1 = 0;

      if (ch1 > 31 && ch2 < 32)
	 ch2 = 0;

      ch1 = _clip_cmptbl[ch1];
      ch2 = _clip_cmptbl[ch2];

      if (ch1 < ch2)
	 return -(ch2 - ch1);
      else if (ch1 > ch2)
	 return ch1 - ch2;
   }

   if (len1)
      return ch1;
   else if (len2)
      return -ch2;
   return 0;
}
