void
_clip_unix_path(char *p, int doslike)
{
#if 1
   int i, j, len = strlen(p);

   char *buf = p;

   for (i = 0, j = 0; i < len; i++)
   {
      if (doslike && buf[i] == ' ')
	 continue;
      if (buf[i] == '\\')
	 buf[j] = '/';
      if (doslike)
	 buf[j] = tolower((unsigned char) (buf[i]));
      j++;
   }
   buf[j] = 0;
#else
   while (*p)
   {
      if (*p == '\\')
	 *p = '/';
      else if (tolow)
	 *p = tolower(*p);
      p++;
   }
#endif
}
