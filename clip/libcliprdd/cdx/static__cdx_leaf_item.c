static void
_cdx_leaf_item(RDD_ORDER * ro, CDX_LEAF * leaf, int pos, unsigned int *recno, void *keyval)
{
   unsigned long long item;

   unsigned int rec = 0;

   int dup, trail;

   char *p = (char *) leaf + sizeof(CDX_LEAF);

   int i;

   for (i = 0; i <= pos; i++)
   {
      memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
      _CDX_LEAF_ITEM(item, rec, dup, trail);
      p -= ro->bufsize - (dup + trail);
      memcpy(keyval + dup, p, ro->bufsize - (dup + trail));
      if (ro->type == 'C' && !ro->binary)
      {
	 memset(keyval + dup + ro->bufsize - (dup + trail), 0x20, trail);
      }
      else
      {
	 memset(keyval + dup + ro->bufsize - (dup + trail), 0x00, trail);
      }
   }
   *recno = rec;
}
