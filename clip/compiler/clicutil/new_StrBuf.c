StrBuf *
new_StrBuf()
{
   NEWVAR(StrBuf, bp);

   bp->buf_of_StrBuf = bp->ptr_of_StrBuf = (char *) malloc(64);
   bp->size_of_StrBuf = 64;
   bp->end_of_StrBuf = bp->buf_of_StrBuf + bp->size_of_StrBuf;

   return bp;
}
