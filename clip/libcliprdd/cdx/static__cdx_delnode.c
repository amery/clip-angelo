static void
_cdx_delnode(RDD_ORDER * ro, CDX_LEAF * leaf, int pos)
{
   void *keys = malloc(_rdd_ushort(leaf->nkeys) * ro->bufsize);

   unsigned int *recnos = malloc(_rdd_ushort(leaf->nkeys) * sizeof(unsigned int));

   unsigned int *lens = malloc(_rdd_ushort(leaf->nkeys) * sizeof(unsigned int));

   int dup, trail;

   unsigned long long item;

   void *p;

   int i, fr;

   _cdx_loadkeys(ro, leaf, keys);

   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
   {
      _cdx_dup_trail_rec(leaf, i, &dup, &trail, (int *) (recnos + i));
      lens[i] = ro->bufsize - trail;
   }
   memmove(keys + pos * ro->bufsize, keys + (pos + 1) * ro->bufsize, (_rdd_ushort(leaf->nkeys) - pos - 1) * ro->bufsize);
   memmove(&recnos[pos], &recnos[pos + 1], (_rdd_ushort(leaf->nkeys) - pos - 1) * sizeof(unsigned int));
   memmove(&lens[pos], &lens[pos + 1], (_rdd_ushort(leaf->nkeys) - pos - 1) * sizeof(unsigned int));

   _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) - 1);
   memset(leaf->keys, 0, sizeof(leaf->keys));
   fr = sizeof(leaf->keys);
   p = (void *) leaf + sizeof(CDX_PAGE);
   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
   {
      trail = ro->bufsize - lens[i];
      item = trail;
      item = item << leaf->dupbits;
      if (i == 0)
      {
	 dup = 0;
      }
      else
      {
	 dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	 item += dup;
      }
      item = item << leaf->recbits;
      item += recnos[i];
      memcpy(leaf->keys + i * leaf->bytes, &item, leaf->bytes);
      p -= ro->bufsize - (dup + trail);
      memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
      fr -= leaf->bytes + ro->bufsize - (dup + trail);
   }
   _rdd_put_ushort(leaf->free, fr);
   free(keys);
   free(recnos);
   free(lens);
}
