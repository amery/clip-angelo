static int
_cdx_search_leaf(RDD_ORDER * ro, CDX_LEAF * leaf, void *key, int len, unsigned int recno, int *pos, int back)
{
   void *val = malloc(ro->bufsize);

   unsigned long long item;

   unsigned int rec;

   int dup, trail;

   char *p = (char *) leaf + sizeof(CDX_LEAF);

   int ret = 0;

   int f = 0;

   if (_rdd_ushort(leaf->nkeys) == 0)
   {
      *pos = 0;
   }
   else
   {
      for ((*pos) = 0; *pos < _rdd_ushort(leaf->nkeys); (*pos)++)
      {
	 int r;

	 memcpy(&item, leaf->keys + (*pos) * leaf->bytes, leaf->bytes);
	 _CDX_LEAF_ITEM(item, rec, dup, trail);
	 p -= ro->bufsize - (dup + trail);
	 memset(val + dup, 0, ro->bufsize - dup);
	 memcpy(val + dup, p, ro->bufsize - (dup + trail));
	 if (ro->type == 'C' && !ro->binary)
	    memset(val + dup + ro->bufsize - (dup + trail), 0x20, trail);
	 r = _cdx_cmp(ro, val, key, len);
	 if (r == 0)
	 {
	    memcpy(ro->key, val, ro->keysize);
	    if (recno)
	    {
	       if (rec == recno)
	       {
		  ret = 1;
		  goto e;
	       }
	       else if (rec > recno)
	       {
		  goto e;
	       }
	    }
	    else
	    {
	       if (back && (*pos < _rdd_ushort(leaf->nkeys) - 1))
	       {
		  f = 1;
		  continue;
	       }
	       ret = 1;
	       goto e;
	    }
	 }
	 else if (r > 0)
	 {
	    if (back)
	       (*pos)--;
	    ret = f;
	    goto e;
	 }
	 else
	 {
	    memcpy(ro->key, val, ro->keysize);
	 }
      }
   }
 e:
   free(val);
   return ret;
}
