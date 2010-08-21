int
_clip_uuencode(char *sstr, long l, char **strp, long *lenp, int without_newline)
{
   OutBuf buf;

   int nl, n, ch, ll;

   unsigned char *str = (unsigned char *) sstr;

   unsigned char *p;

   init_Buf(&buf);
   nl = l / 45;
   for (n = 0, p = str; n < l; n += 45)
   {
      int nn;

      unsigned char c1, c2;

      ll = (l - n < 45) ? (l - n) : 45;
      putByte_Buf(&buf, ENC(ll));

      for (nn = ll; nn > 2; nn -= 3, p += 3)
      {
	 ch = *p >> 2;
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);
	 ch = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);
	 ch = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);
	 ch = p[2] & 077;
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);
      }

      if (nn)
      {
	 c1 = *p;
	 c2 = (nn == 1) ? 0 : p[1];

	 ch = c1 >> 2;
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);

	 ch = ((c1 << 4) & 060) | ((c2 >> 4) & 017);
	 ch = ENC(ch);
	 putByte_Buf(&buf, ch);

	 if (nn == 1)
	    ch = ENC('\0');
	 else
	 {
	    ch = (c2 << 2) & 074;
	    ch = ENC(ch);
	 }
	 putByte_Buf(&buf, ch);
	 ch = ENC('\0');
	 putByte_Buf(&buf, ch);
	 if (!without_newline)
	    putByte_Buf(&buf, '\n');
      }
      else if (!without_newline)
	 putByte_Buf(&buf, '\n');
   }

   ch = ENC('\0');
   putByte_Buf(&buf, ch);
   if (!without_newline)
      putByte_Buf(&buf, '\n');

   *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   *strp = (char *) realloc(*strp, *lenp + 1);
   memcpy(*strp, buf.buf_of_OutBuf, *lenp);
   (*strp)[*lenp] = 0;

   destroy_Buf(&buf);
   return 0;
}
