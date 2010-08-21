void
putByte_Buf(OutBuf * bp, unsigned char b)
{
   *bp->ptr_of_OutBuf = b;
   ++(bp->ptr_of_OutBuf);
   if (bp->ptr_of_OutBuf >= bp->end_of_OutBuf)
      grow_buf(bp, 0);
}
