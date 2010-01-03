static void
grow_buf(StrBuf * bp, int min)
{
	int       dsize = bp->size_of_StrBuf / 2;

	char     *buf = bp->buf_of_StrBuf;

   if (min < 16)
      min = 16;
   if (dsize < min)
      dsize = min;
	bp->size_of_StrBuf = bp->size_of_StrBuf + dsize;

	bp->buf_of_StrBuf = realloc(bp->buf_of_StrBuf, bp->size_of_StrBuf);

	bp->end_of_StrBuf = bp->buf_of_StrBuf + bp->size_of_StrBuf;
   if (buf)
		bp->ptr_of_StrBuf = bp->buf_of_StrBuf + (bp->ptr_of_StrBuf - buf);
   else
		bp->ptr_of_StrBuf = bp->buf_of_StrBuf;
}
