void
putByte_StrBuf(StrBuf * bp, unsigned char b)
{
   if (!bp->ptr_of_StrBuf || bp->ptr_of_StrBuf >= bp->end_of_StrBuf)
      grow_buf(bp, 1);
   *bp->ptr_of_StrBuf = b;
   ++bp->ptr_of_StrBuf;
}
