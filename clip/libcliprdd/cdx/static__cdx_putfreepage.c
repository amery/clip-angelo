static int
_cdx_putfreepage(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   char fuu[4];

   CDX_PAGE fp;

   int er;

   memset(&fp, 0, sizeof(CDX_PAGE));
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 4, 4, fuu, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 4, 4, &page, __PROC__)))
      return er;
   _rdd_put_uint((unsigned char *) &fp, _rdd_uint((unsigned char *) fuu));
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_PAGE), &fp, __PROC__)))
      return er;
   return 0;
}
