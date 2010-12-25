static void
_cdx_init_leaf(RDD_ORDER * ro, CDX_LEAF * leaf, unsigned int lastrec, unsigned int left)
{
   int i, bits = 1;

   unsigned int mask;

   _rdd_put_ushort(leaf->attrs, (left == 0xffffffff) ? 3 : 2);
   _rdd_put_ushort(leaf->nkeys, 0);
   _rdd_put_uint(leaf->left, left);
   _rdd_put_uint(leaf->right, 0xffffffff);
   _rdd_put_ushort(leaf->free, sizeof(leaf->keys));

   for (i = 1; bits < ro->bufsize; bits = (bits << 1) + 1, i++);
   leaf->dupbits = leaf->trailbits = i;
   leaf->dupmask = leaf->trailmask = bits;
   i = 0;
   lastrec = max(lastrec, 256);
   while (_cdx_bits[i++] < lastrec);
   i += leaf->dupbits + leaf->trailbits;
   if (i % 8)
      i = i / 8 + 1;
   else
      i = i / 8;

   leaf->bytes = i;
   leaf->recbits = leaf->bytes * 8 - (leaf->dupbits + leaf->trailbits);
   mask = 1;
   for (i = 1; i < leaf->recbits; i++)
      mask = (mask << 1) + 1;
   _rdd_put_uint(leaf->recmask, mask);

   memset(leaf->keys, 0, sizeof(leaf->keys));
}
