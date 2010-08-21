static int
_clip_addExtToFile(char *buf, int maxlen, const char *sExt)
{
   int len = strlen(buf);

   int len2 = strlen(sExt);

   int i = len, j;

   int extExist = 0;

   /* filename begin from end string */
   for (; i >= 0; i--)
   {
      if (buf[i] == '/')
	 break;
      if (buf[i] == '\\')
	 break;
      if (buf[i] == ':')
	 break;
   }
   /* ext exist ? */
   for (; i < len; i++)
   {
      if (buf[i] == '.')
	 extExist = 1;
   }
   i = len;
   if (!extExist && i < (maxlen - (len2 + 1)))
   {
      buf[i] = '.';
      for (j = 0, i++; j < len2; j++, i++)
	 buf[i] = sExt[j];
      buf[i] = 0;
      extExist = 1;
   }
   else
      extExist = 0;
   return extExist;
}
