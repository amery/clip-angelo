void
delete_StrBuf(StrBuf * bp)
{
   if (!bp)
      return;
	free(bp->buf_of_StrBuf);
   free(bp);
}
