static void
put_Buf(Buf * bp, int ch)
{
	if (bp->end_of_Buf == bp->size_of_Buf)
   {
      int delta;

		delta = bp->size_of_Buf / 4;
      if (delta < 32)
	 delta = 32;
	 bp->size_of_Buf += delta;
	 bp->ptr_of_Buf = (char *) realloc(bp->ptr_of_Buf, bp->size_of_Buf);
   }

   bp->ptr_of_Buf[bp->end_of_Buf++] = ch;
}
