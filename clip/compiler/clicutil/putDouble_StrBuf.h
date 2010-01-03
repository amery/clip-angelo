void
putDouble_StrBuf(StrBuf * bp, double d)
{
  /*unsigned char buf[sizeof(double)]; */

   write_StrBuf(bp, &d, sizeof(d));
}
