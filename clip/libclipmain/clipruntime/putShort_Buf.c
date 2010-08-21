
void
putShort_Buf(OutBuf * bp, short s)
{
   putBuf_Buf(bp, (char *) &s, sizeof(s));
}
