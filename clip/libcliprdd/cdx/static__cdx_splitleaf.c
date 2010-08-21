static int
_cdx_splitleaf(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * old, CDX_LEAF * new, unsigned int oldpage, unsigned int newpage, void *keys, const char *__PROC__)
{
   int *lens = malloc(_rdd_ushort(old->nkeys) * sizeof(int));

   unsigned int *recnos = malloc(_rdd_ushort(old->nkeys) * sizeof(unsigned int));

   int dup, trail, i, er, fr;

   unsigned long long item;

   void *p;

   int c1 = _rdd_ushort(old->nkeys) / 2;

   int c2 = _rdd_ushort(old->nkeys) - c1;

   for (i = 0; i < _rdd_ushort(old->nkeys); i++)
   {
      _cdx_dup_trail_rec(old, i, &dup, &trail, (int *) (recnos + i));
      lens[i] = ro->bufsize - trail;
   }

   _rdd_put_ushort(old->attrs, 2);
   _rdd_put_ushort(new->attrs, 2);
   _rdd_put_uint(new->left, oldpage);
   _rdd_put_uint(new->right, _rdd_uint(old->right));
   _rdd_put_uint(old->right, newpage);
   if (_rdd_uint(new->right) != 0xffffffff)
   {
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(new->right) + 4, 4, &newpage, __PROC__)))
	 goto err;
   }
   _rdd_put_ushort(old->nkeys, c1);
   _rdd_put_ushort(new->nkeys, c2);
   memcpy(new->recmask, old->recmask, 10);	/* masks, bits, bytes ... */

   memset(old->keys, 0, sizeof(old->keys));
   fr = sizeof(old->keys);
   p = (void *) old + sizeof(CDX_PAGE);
   for (i = 0; i < c1; i++)
   {
      trail = ro->bufsize - lens[i];
      item = trail;
      item = item << old->dupbits;
      if (i == 0)
      {
	 dup = 0;
      }
      else
      {
	 dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	 item += dup;
      }
      item = item << old->recbits;
      item += recnos[i];
      memcpy(old->keys + i * old->bytes, &item, old->bytes);
      p -= ro->bufsize - (dup + trail);
      memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
      fr -= old->bytes + ro->bufsize - (dup + trail);
   }
   _rdd_put_ushort(old->free, fr);

   memset(new->keys, 0, sizeof(new->keys));
   fr = sizeof(new->keys);
   p = (void *) new + sizeof(CDX_PAGE);
   for (; i < c1 + c2; i++)
   {
      trail = ro->bufsize - lens[i];
      item = trail;
      item = item << new->dupbits;
      if (i == c1)
      {
	 dup = 0;
      }
      else
      {
	 dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	 item += dup;
      }
      item = item << new->recbits;
      item += recnos[i];
      memcpy(new->keys + (i - c1) * new->bytes, &item, new->bytes);
      p -= ro->bufsize - (dup + trail);
      memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
      fr -= new->bytes + ro->bufsize - (dup + trail);
   }
   _rdd_put_ushort(new->free, fr);
   free(lens);
   free(recnos);
   return 0;
 err:
   free(lens);
   free(recnos);
   return er;
}
