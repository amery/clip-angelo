static int
__cdx_sets(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, const char *__PROC__)
{
   CDX_BRANCH branch;

   int er;

   if (level < 0)
      return 0;
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   memcpy(branch.keys + (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8), key + 4, ro->bufsize);
   _rdd_put_backuint(branch.keys + (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8) + ro->bufsize, _rdd_uint(key));
   _rdd_put_backuint(branch.keys + (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8) + ro->bufsize + 4, ro->stack[level + 1].page);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   return __cdx_sets(ClipMachineMemory, ro, level - 1, key, __PROC__);
}
