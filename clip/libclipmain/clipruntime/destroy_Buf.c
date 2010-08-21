void
destroy_Buf(OutBuf * bp)
{
   if (!bp)
      return;
   if (bp->buf_of_OutBuf)
      free(bp->buf_of_OutBuf);
}
