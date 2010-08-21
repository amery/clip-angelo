void
write_StrBuf(StrBuf * bp, void *p, int len)
{
   if (!bp->buf_of_StrBuf || (bp->end_of_StrBuf - bp->ptr_of_StrBuf) <= len)
      grow_buf(bp, len + 1);
   memcpy(bp->ptr_of_StrBuf, p, len);
   bp->ptr_of_StrBuf += len;
}
