long
_clip_str_to_time(char *str)
{
   int len, am = 0, pm = 0;

   char *t = str, *e, *ms = 0, *ss = 0, *ds = 0;

   long r;

   int h = 0, m = 0, s = 0, d = 0;

   if (!str)
      return 0;

   while (*t == ' ')
      t++;
   ms = strchr(t, ':');
   if (ms)
   {
      ss = strchr(ms + 1, ':');
      if (ss)
	 ds = strchr(ss + 1, ':');
   }
   len = strlen(t);
   e = t + len - 1;
   while (*e == ' ')
   {
      e--;
      len--;
   }

   h = atoi(t);
   if (ms)
      m = atoi(ms + 1);
   if (ss)
      s = atoi(ss + 1);
   if (ds)
      d = atoi(ds + 1);

   if (toupper(*e) == 'M')
   {
      if (toupper(*(e - 1)) == 'A')
	 am = 1;
      else if (toupper(*(e - 1)) == 'P')
	 pm = 1;
      else
	 return -1;
   }
   if ((am || pm) && (h > 12))
      return -1;

   if (h == 12 && (am || pm))
      h = 0;
   if (pm)
      h += 12;
   if (h == 24)
      h = 0;

   if (h < 0 || h > 23)
      return -1;
   if (m < 0 || m > 59)
      return -1;
   if (s < 0 || s > 59)
      return -1;
   if (d < 0 || d > 99)
      return -1;

   r = h * 60 * 60 * 1000 + m * 60 * 1000 + s * 1000 + d * 10;
   return r;
}
