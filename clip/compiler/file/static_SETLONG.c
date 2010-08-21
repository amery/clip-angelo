static void
SETLONG(StrBuf * bp, long offs, long val)
{
   memcpy(bp->buf + offs, &val, sizeof(long));
}
