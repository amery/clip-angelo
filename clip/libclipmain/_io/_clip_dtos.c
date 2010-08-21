void
_clip_dtos(double d, char *buf, int buflen, int *dp)
{
   int dl;

   char *s;

   snprintf(buf, buflen, "%.0f", d);
   dl = strlen(buf);
   if (dp)
      *dp = dl;
   snprintf(buf, buflen, "%.*f", buflen - dl - 1, d);

   for (s = buf + strlen(buf); s > buf; --s)
      if ((*s) == ',')
	 (*s) = '.';
   for (s = buf + strlen(buf); s > buf; --s)
      if (s[-1] != '0')
	 break;
   if (s > buf && s[-1] == '.')
      --s;
   *s = 0;
}
