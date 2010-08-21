static int
ntx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_PAGE page;

   int er;

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(NTX_HEADER) + sizeof(NTX_PAGE), __PROC__)))
      return er;
   _ntx_header(ri->orders[0], &hdr);
   _rdd_put_uint(hdr.fuu, 0);
   _rdd_put_uint(hdr.root, sizeof(NTX_HEADER));
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _ntx_init_page(ClipMachineMemory, ri->orders[0], &page, 0, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ri->file, sizeof(NTX_HEADER), sizeof(NTX_PAGE), (char *) &page, __PROC__)))
      return er;
   rd->bof = rd->v_bof = rd->eof = 1;
   return rd->vtbl->rawgo(ClipMachineMemory, rd, 1, 0, __PROC__);
}
