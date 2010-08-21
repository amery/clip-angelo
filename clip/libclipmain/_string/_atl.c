const char *
_atl(const char *src, const char *dst, int slen, int dlen)
{
   const char *end, *de, *s = src, *d = dst;

   for (end = src + slen, de = dst + dlen - 1; src < end; src++)
      if (*src == *d)
      {
	 s = src;
	 for (; src < end && d < de && *(++src) == *(++d););
	 if (d == de && *src == *d)
	    return s;
	 src = s;
	 d = dst;
      }
   return 0;
}
