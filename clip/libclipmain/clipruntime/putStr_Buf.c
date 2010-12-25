void
putStr_Buf(OutBuf * bp, char *str)
{
   putBuf_Buf(bp, str, strlen(str) + 1);
}
