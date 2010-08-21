static int
__cdx_adds(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, unsigned int *newroot, const char *__PROC__)
{
   CDX_BRANCH branch;

   int er;

   if (level < 0)
   {
      CDX_BRANCH b;

      if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, newroot, __PROC__)))
	 return er;
      _cdx_init_branch(ro, &branch, 0xffffffff);

      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[0].page, sizeof(CDX_BRANCH), &b, __PROC__)))
	 return er;
      if (!(_rdd_ushort(b.attrs) & 0x02))
      {
	 unsigned int page = _rdd_uint(b.left);

	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_BRANCH), &b, __PROC__)))
	    return er;
	 memcpy(branch.keys, b.keys + (_rdd_ushort(b.nkeys) - 1) * (ro->bufsize + 8), ro->bufsize + 4);
	 _rdd_put_backuint(branch.keys + ro->bufsize + 4, page);
	 _rdd_put_ushort(branch.nkeys, 1);
      }
   }
   else
   {
      unsigned int newbranch;

      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
	 return er;
      if ((_rdd_ushort(branch.nkeys) + 1) * (ro->bufsize + 8) > sizeof(branch.keys))
      {
	 if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newbranch, __PROC__)))
	    return er;
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page + 8, 4, &newbranch, __PROC__)))
	    return er;
	 _cdx_init_branch(ro, &branch, ro->stack[level].page);
	 ro->stack[level].page = newbranch;
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
	    return er;
	 if ((er = __cdx_adds(ClipMachineMemory, ro, level - 1, key, newroot, __PROC__)))
	    return er;
      }
   }
   memcpy(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8), key + 4, ro->bufsize);
   _rdd_put_backuint(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8) + ro->bufsize, _rdd_uint(key));
   _rdd_put_backuint(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8) + ro->bufsize + 4, ro->stack[level + 1].page);
   if ((er = __cdx_sets(ClipMachineMemory, ro, level - 1, key, __PROC__)))
      return er;
   _rdd_put_ushort(branch.nkeys, _rdd_ushort(branch.nkeys) + 1);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, (level < 0) ? *newroot : ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   return 0;
}
