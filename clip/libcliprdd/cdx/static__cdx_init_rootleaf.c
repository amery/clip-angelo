static int
_cdx_init_rootleaf(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   CDX_LEAF leaf;

   int i, bits = 1, er;

   unsigned int mask;

   _rdd_put_ushort(leaf.attrs, 3);
   _rdd_put_ushort(leaf.nkeys, 0);
   _rdd_put_uint(leaf.left, 0xffffffff);
   _rdd_put_uint(leaf.right, 0xffffffff);
   _rdd_put_ushort(leaf.free, sizeof(leaf.keys));

   for (i = 1; bits < ro->bufsize; bits = (bits << 1) + 1, i++);
   leaf.dupbits = leaf.trailbits = i;
   leaf.dupmask = leaf.trailmask = bits;

   leaf.bytes = 3;
   leaf.recbits = leaf.bytes * 8 - (leaf.dupbits + leaf.trailbits);
   mask = 1;
   for (i = 1; i < leaf.recbits; i++)
      mask = (mask << 1) + 1;
   _rdd_put_uint(leaf.recmask, mask);

   memset(leaf.keys, 0, sizeof(leaf.keys));

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_LEAF), &leaf, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, 4, &page, __PROC__)))
      return er;
   return 0;
}
