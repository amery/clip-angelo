static int
_ntx_put_free_page(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   NTX_FREEPAGE fp;

   NTX_HEADER hdr;

   unsigned int freepage;

   int er;

   _ntx_header(ro, &hdr);
   freepage = _rdd_uint(hdr.fuu);

   _ntx_page(ro, page, (NTX_PAGE *) & fp);
   _rdd_put_backuint(fp.next, freepage);
   _rdd_put_uint(hdr.fuu, page);

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(NTX_FREEPAGE), (char *) &fp, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   return 0;
}
