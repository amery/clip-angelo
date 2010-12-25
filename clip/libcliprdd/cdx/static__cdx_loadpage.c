static int
_cdx_loadpage(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int offs, const char *__PROC__)
{
   if (offs == ro->curoffs)
      return 0;
   ro->curoffs = offs;
   return rdd_read(ClipMachineMemory, &ro->index->file, offs, sizeof(CDX_PAGE), (char *) ro->curpage, __PROC__);
}
