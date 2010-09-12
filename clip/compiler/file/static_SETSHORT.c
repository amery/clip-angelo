static void
SETSHORT(StrBuf * bp, long offs, short val)
{
   memcpy(bp->buf + offs, &val, sizeof(short));
}
