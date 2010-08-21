static int
_cdx_keyno(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int npage, int pos, unsigned int *keyno, const char *__PROC__)
{
   CDX_PAGE page;

   int er;

   if (!ro->descend)
   {
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	 return er;
      npage = _rdd_uint(page.left);
      *keyno = pos + 1;
      while (npage != 0xffffffff)
      {
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	    return er;
	 npage = _rdd_uint(page.left);
	 *keyno += _rdd_ushort(page.nkeys);
      }
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	 return er;
      npage = _rdd_uint(page.right);
      *keyno = _rdd_ushort(page.nkeys) - pos;
      while (npage != 0xffffffff)
      {
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	    return er;
	 npage = _rdd_uint(page.right);
	 *keyno += _rdd_ushort(page.nkeys);
      }
   }
   return 0;
}
