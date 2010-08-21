void
putInt_StrBuf(StrBuf * bp, int i)
{
   long l = i;

   write_StrBuf(bp, &l, sizeof(l));
}
