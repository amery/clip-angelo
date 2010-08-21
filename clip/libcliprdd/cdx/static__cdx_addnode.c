static void
_cdx_addnode(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys, int pos, void *key, int len, unsigned int recno, int addbytes, int fr)
{
   int i;

   int dup, trail;

   unsigned long long item;

   /*if(addbytes) */
   {
      int *lens = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      unsigned int *recnos = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      int *dups = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      void *p;

      for (i = 0; i < pos; i++)
      {
	 memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
	 _CDX_LEAF_ITEM(item, recnos[i], dup, trail);
	 lens[i] = ro->bufsize - trail;
	 dups[i] = dup;
      }
      recnos[i] = recno;
      lens[i] = len;
      dups[i] = 0;
      for (i = pos; i < _rdd_ushort(leaf->nkeys); i++)
      {
	 memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
	 _CDX_LEAF_ITEM(item, recnos[i + 1], dup, trail);
	 lens[i + 1] = ro->bufsize - trail;
	 dups[i + 1] = dup;
      }
      memmove(keys + (pos + 1) * ro->bufsize, keys + pos * ro->bufsize, (_rdd_ushort(leaf->nkeys) - pos) * ro->bufsize);
      memcpy(keys + pos * ro->bufsize, key, len);
      memset(keys + pos * ro->bufsize + len, 0, ro->bufsize - len);

      if (addbytes)
      {
	 unsigned int bits = 1;

	 leaf->bytes += addbytes;
	 leaf->recbits += addbytes * 8;
	 for (i = 1; i < leaf->recbits; i++)
	    bits = (bits << 1) + 1;
	 _rdd_put_uint(leaf->recmask, bits);
      }

      memset(leaf->keys, 0, sizeof(leaf->keys));
      _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) + 1);
      _rdd_put_ushort(leaf->free, fr);
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
	    if (i == pos || i == pos + 1)
	       dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, min(lens[i - 1], lens[i]));
	    else
	       dup = dups[i];
	    item += dup;
	 }
	 item = item << leaf->recbits;
	 item += recnos[i];
	 memcpy(leaf->keys + i * leaf->bytes, &item, leaf->bytes);
	 p -= ro->bufsize - (dup + trail);
	 memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
      }
      free(lens);
      free(recnos);
      free(dups);
   }				/* else {
				   unsigned int rec;
				   int l,db;
				   void* val = malloc(ro->bufsize);
				   void* be = (void*)leaf+sizeof(CDX_PAGE);
				   void* ab;

				   for(i=0;i<pos;i++){
				   item = *(unsigned long long*)(leaf->keys+i*leaf->bytes);
				   _CDX_LEAF_ITEM(item,rec,dup,trail);
				   l = ro->bufsize - (dup+trail);
				   be -= l;
				   memcpy(val+dup,be,l);
				   }
				   item = *(unsigned long long*)(leaf->keys+i*leaf->bytes);
				   _CDX_LEAF_ITEM(item,rec,dup,trail);
				   ab = pos==_rdd_ushort(leaf->nkeys)?0:be-(ro->bufsize - (dup+trail));
				   memmove(leaf->keys+(pos+1)*leaf->bytes,leaf->keys+pos*leaf->bytes,
				   (_rdd_ushort(leaf->nkeys)-pos)*leaf->bytes);
				   db = _cdx_dupbytes(key,val,min(len,ro->bufsize-trail));
				   if(ab)
				   memmove(be-(len-db),ab,
				   (int)(ab-(void*)leaf->keys)-
				   (_rdd_ushort(leaf->nkeys)+1)*leaf->bytes);
				   memcpy(be-(len-db),key+db,len-db);
				   item = ro->bufsize-len;
				   item = item << leaf->dupbits;
				   item += db;
				   item = item << leaf->recbits;
				   item += recno;
				   memcpy(leaf->keys+pos*leaf->bytes,&item,leaf->bytes);
				   _rdd_put_ushort(leaf->nkeys,_rdd_ushort(leaf->nkeys)+1);
				   _rdd_put_ushort(leaf->free,fr);
				   free(val);
				   } */
}
