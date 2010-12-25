static int
_cdx_leaf2split(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys, int pos, void *key, int len, unsigned int recno, int *addbytes)
{
   int free = _rdd_ushort(leaf->free);

   int dup, trail = 0;

   *addbytes = 0;
   free -= leaf->bytes;
   if ((recno & _rdd_uint(leaf->recmask)) != recno)
   {
      int i = 0;

      while (_cdx_bits[i++] < recno);
      i -= leaf->recbits;
      if (i % 8)
	 i = i / 8 + 1;
      else
	 i = i / 8;
      free -= (_rdd_ushort(leaf->nkeys) + 1) * i;
      *addbytes = i;
   }
   if (free < 0)
      return free;

   /* free += */
   if (pos < _rdd_ushort(leaf->nkeys))
   {
      _cdx_dup_trail_rec(leaf, pos, &dup, &trail, NULL);
      free += ro->bufsize - (dup + trail);
   }
   /* free -= */
   if (pos > 0)
      free -= len - _cdx_dupbytes(keys + (pos - 1) * ro->bufsize, key, len);
   else
      free -= len;
   if (pos < _rdd_ushort(leaf->nkeys))
      free -= (ro->bufsize - trail) - _cdx_dupbytes(keys + pos * ro->bufsize, key, len);

   return free;
}
