static void
_cdx_dup_trail_rec(CDX_LEAF * leaf, int pos, int *dup, int *trail, int *rec)
{
   unsigned long long item;

   unsigned int _rec;

   int _dup, _trail;

   memcpy(&item, leaf->keys + pos * leaf->bytes, leaf->bytes);
   _CDX_LEAF_ITEM(item, _rec, _dup, _trail);
   *dup = _dup;
   *trail = _trail;
   if (rec)
      *rec = _rec;
}
