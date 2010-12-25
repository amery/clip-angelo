static int
_cdx_adds(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key, const char *__PROC__)
{
   unsigned int newroot = 0;

   int er;

   if ((er = __cdx_adds(ClipMachineMemory, ro, ro->level - 1, key, &newroot, __PROC__)))
      return er;
   if (newroot)
   {
      int i;

      for (i = MAX_BTREE_DEEP - 1; i > 0; i--)
	 ro->stack[i] = ro->stack[i - 1];
      ro->stack[0].page = newroot;
      ro->level++;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(unsigned int), &newroot, __PROC__)))
	 return er;
   }
   return 0;
}
