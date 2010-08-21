void
putShort_StrBuf(StrBuf * bp, short s)
{
   write_StrBuf(bp, &s, sizeof(s));
}
