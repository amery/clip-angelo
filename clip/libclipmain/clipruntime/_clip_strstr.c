const char *
_clip_strstr(const char *src, int slen, const char *dst, int dlen)
{
   const char *end, *de, *s = src, *d = dst;

   /*
      if(!dlen)
      return src;
    */
   for (end = src + slen, de = dst + dlen - 1; src < end; src++)
      if (*src == *d)
      {
	 s = src;
	 for (; src < end && d < de && *(++src) == *(++d);)
	    ;
	 if (d == de && *src == *d)
	    return s;
	 src = s;
	 d = dst;
      }
   return 0;
}
