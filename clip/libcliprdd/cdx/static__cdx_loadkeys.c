static void
_cdx_loadkeys(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys)
{
   unsigned long long item;

   unsigned int rec;

   int dup, trail;

   char *p = (char *) leaf + sizeof(CDX_LEAF);

   int i;

   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
   {
      memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
      _CDX_LEAF_ITEM(item, rec, dup, trail);
      p -= ro->bufsize - (dup + trail);
      if (i)
	 memcpy(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, ro->bufsize);
      memcpy(keys + i * ro->bufsize + dup, p, ro->bufsize - (dup + trail));
      memset(keys + i * ro->bufsize + dup + ro->bufsize - (dup + trail), 0x0, trail);
   }
}
