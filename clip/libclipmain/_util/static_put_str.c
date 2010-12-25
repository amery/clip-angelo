static void
put_str(OutBuf * bp, char *str, long len)
{
   putInt32_Buf(bp, htonl(len));
   putBuf_Buf(bp, str, len);
}
