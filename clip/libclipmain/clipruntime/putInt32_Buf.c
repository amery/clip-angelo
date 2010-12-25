void
putInt32_Buf(OutBuf * bp, long l)
{
   int r = (int) l;

   putBuf_Buf(bp, (char *) &r, sizeof(r));
}
