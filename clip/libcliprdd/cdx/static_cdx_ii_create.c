static int
cdx_ii_create(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   unsigned int root;

   int er;

   ri->loc = dbf_get_locale(ClipMachineMemory);
   root = sizeof(CDX_HEADER);
   memset(&chdr, 0, sizeof(CDX_HEADER));
   _rdd_put_uint(chdr.root, sizeof(CDX_HEADER));
   _rdd_put_ushort(chdr.keysize, 10);
   chdr.options = 0xE0;
   chdr.sign = 1;
   _rdd_put_ushort(chdr.totlen, 1);
   _rdd_put_ushort(chdr.forlen, 1);
   _rdd_put_ushort(chdr.explen, 1);
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &root, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, root, __PROC__)))
      goto err;

   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
