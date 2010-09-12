static void
setshort(int line, StrBuf * bp, long offs, long val)
{
   if (val > 32767 || val < -32767 /*val & 0xffff0000 */ )
   {
      yyerror("po code too large at line %d", line);
   }
   (*(short *) ((bp)->buf_of_StrBuf + (offs)) = (val & 0xffff));
}
