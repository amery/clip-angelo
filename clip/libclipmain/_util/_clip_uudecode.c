int
_clip_uudecode(char *sstr, long l, char **strp, long *lenp)
{
   OutBuf buf;

   int n, ch;

   unsigned char *str = (unsigned char *) sstr;

   unsigned char *p, *e;

   init_Buf(&buf);

   for (p = str, e = str + l; p < e;)
   {
      if (*p == '\n')
      {
	 p++;
	 continue;
      }

      n = DEC(*p);
      if (n < 0)
	 return -1;

      for (++p; n > 0; p += 4, n -= 3)
      {

	 if (p >= e)
	    return -1;

	 if (n >= 3)
	 {
	    if (p + 3 >= e)
	       return -1;

	    ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
	    putByte_Buf(&buf, ch);
	    ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
	    putByte_Buf(&buf, ch);
	    ch = DEC(p[2]) << 6 | DEC(p[3]);
	    putByte_Buf(&buf, ch);
	 }
	 else
	 {
	    if (n >= 1)
	    {
	       if (p + 1 >= e)
		  return -1;
	       ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
	       putByte_Buf(&buf, ch);
	    }
	    if (n >= 2)
	    {
	       if (p + 2 >= e)
		  return -1;
	       ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
	       putByte_Buf(&buf, ch);
	    }
	 }
      }
   }

   *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   *strp = (char *) realloc(*strp, *lenp + 1);
   memcpy(*strp, buf.buf_of_OutBuf, *lenp);
   (*strp)[*lenp] = 0;

   destroy_Buf(&buf);
   return 0;
}
