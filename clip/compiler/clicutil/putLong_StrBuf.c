void
putLong_StrBuf(StrBuf * bp, long i)
{
   long l = i;

   write_StrBuf(bp, &l, sizeof(l));
}
