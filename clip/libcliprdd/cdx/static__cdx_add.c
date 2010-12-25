static int
_cdx_add(RDD_ORDER * ro, CDX_LEAF * leaf, void *key, void **pos, void *buf)
{
   int dup, trail;

   int len = ro->bufsize;

   unsigned long long item;

   if (ro->type == 'C' || (ro->type == 'X' && ((char *) key)[4] == type_weight(CHARACTER_type_of_ClipVarType)))
   {
      if (ro->binary)
      {
	 while ((len > 0) && (((char *) key)[len + sizeof(unsigned int) - 1] == 0))
	    len--;
      }
      else
      {
	 loc_write(ro->index->loc, key + sizeof(unsigned int), ro->bufsize);
	 while ((len > 0) && (((char *) key)[len + sizeof(unsigned int) - 1] == (ro->type == 'X' ? 0 : ' ')))
	    len--;
      }
   }
   else				/*if(ro->type == 'N' || ro->type == 'D' || ro->type == 'T') */
   {
      char *s = key + sizeof(unsigned int) + ro->bufsize - 1;

      while ((s >= (char *) key + sizeof(unsigned int)) && (*s == 0))
	 s--;
      len = s - ((char *) key + sizeof(unsigned int)) + 1;
   }
   if (_rdd_ushort(leaf->nkeys) == 0)
   {
      dup = 0;
      trail = ro->bufsize - len;
   }
   else
   {
      unsigned int _rec;

      int _dup, _trail;

      memcpy(&item, leaf->keys + (_rdd_ushort(leaf->nkeys) - 1) * leaf->bytes, leaf->bytes);
      _CDX_LEAF_ITEM(item, _rec, _dup, _trail);
      dup = _cdx_dupbytes(buf, key + sizeof(unsigned int), min(ro->bufsize - _trail, len));
      trail = ro->bufsize - len;
   }
   memcpy(buf + dup, key + sizeof(unsigned int) + dup, len - dup);
   *pos -= len - dup;
   memcpy(*pos, buf + dup, len - dup);

   item = trail;
   item = item << leaf->dupbits;
   item += dup;
   item = item << leaf->recbits;
   item += _rdd_uint(key);

   memcpy(leaf->keys + _rdd_ushort(leaf->nkeys) * leaf->bytes, &item, leaf->bytes);
   _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) + 1);
   _rdd_put_ushort(leaf->free, _rdd_ushort(leaf->free) - (leaf->bytes + len - dup));
   return _rdd_ushort(leaf->free);
}
