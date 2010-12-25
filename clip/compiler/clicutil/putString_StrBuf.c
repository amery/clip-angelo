void
putString_StrBuf(StrBuf * bp, const char *str)
{
   write_StrBuf(bp, (void *) str, strlen(str));
}
