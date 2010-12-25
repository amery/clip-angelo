static int
idx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *ro = ri->orders[0];

   int er;

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   _rdd_put_uint(chdr.fuu, 0);
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
