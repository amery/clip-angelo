static void
setshort(int line, StrBuf * bp, long offs, long lval)
{
   short     val;

   if (lval > 32767 || lval < -32767)
    {
       yyerror("po code too large at line %d", line);
    }
   val = lval;
	memcpy(bp->buf_of_StrBuf + offs, (char *) &val, sizeof(short));
}
