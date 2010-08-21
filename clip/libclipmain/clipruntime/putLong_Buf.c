void
putLong_Buf(OutBuf * bp, long l)
{
   putBuf_Buf(bp, (char *) &l, sizeof(l));
}
