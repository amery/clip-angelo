void
putBuf_Buf(OutBuf * bp, char *p, int len)
{
   if ((bp->end_of_OutBuf - bp->ptr_of_OutBuf) <= len)
      grow_buf(bp, len + 1);
   memcpy(bp->ptr_of_OutBuf, p, len);
   bp->ptr_of_OutBuf += len;
}
