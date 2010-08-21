void
putDouble_Buf(OutBuf * bp, double d)
{
   putBuf_Buf(bp, (char *) &d, sizeof(d));
}
