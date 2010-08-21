static void
grow_buf(OutBuf * bp, int min)
{
   int size = bp->end_of_OutBuf - bp->buf_of_OutBuf;

   int dsize = size / 2;

   int pos = bp->ptr_of_OutBuf - bp->buf_of_OutBuf;

   if (min < 16)
      min = 16;
   if (dsize < min)
      dsize = min;
   size += dsize;
   bp->buf_of_OutBuf = realloc(bp->buf_of_OutBuf, size);
   bp->end_of_OutBuf = bp->buf_of_OutBuf + size;
   bp->ptr_of_OutBuf = bp->buf_of_OutBuf + pos;
}
