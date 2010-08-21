static int
cdx_ii_delkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int found, er;

   ro->level = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->iikey, ro->bufsize, *(unsigned int *) ro->id, &found, NULL, __PROC__)))
      return er;
   if (!found)
      return 0;

   return _cdx_delkey(ClipMachineMemory, ro, __PROC__);
}
