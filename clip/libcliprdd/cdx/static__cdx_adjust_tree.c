static int
_cdx_adjust_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, unsigned int rec, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int s, er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   s = ro->stack[level].pos * (ro->bufsize + 8);
   memcpy(branch->keys + s, key, ro->bufsize);
   _rdd_put_backuint(branch->keys + s + ro->bufsize, rec);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
      return er;
   if (level && (ro->stack[level].pos == _rdd_ushort(branch->nkeys) - 1))
   {
      if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1,
				 branch->keys + ro->stack[level].pos * (ro->bufsize + 8), _rdd_backuint(branch->keys + ro->stack[level].pos * (ro->bufsize + 8) + ro->bufsize), __PROC__)))
	 return er;
   }
   return 0;
}
