const char *
_atr(const char *src, const char *dst, int slen, int dlen)
{
   const char *s = src + slen - 1, *d = dst + dlen - 1, *ss = s;

   for (; s >= src; s--)
      if (*s == *d)
      {
	 ss = s;
	 for (; s >= src && d >= dst && *(--s) == *(--d););
	 if (d == dst - 1)
	    return s + 1;
	 s = ss;
	 d = dst + dlen - 1;
      }
   return 0;
}
