static int
_cdx_delstick(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (_rdd_ushort(branch->nkeys) == 1)
   {
      if (_rdd_uint(branch->left) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(branch->left) + 8, 4, branch->right, __PROC__)))
	    return er;
      }
      if (_rdd_uint(branch->right) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(branch->right) + 4, 4, branch->left, __PROC__)))
	    return er;
      }
      if (level > 0)
      {
	 if ((er = _cdx_putfreepage(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
	    return er;
	 if ((er = _cdx_delstick(ClipMachineMemory, ro, level - 1, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
	    return er;
      }
   }
   else
   {
      memmove(branch->keys + ro->stack[level].pos * (ro->bufsize + 8),
	      branch->keys + (ro->stack[level].pos + 1) * (ro->bufsize + 8), (_rdd_ushort(branch->nkeys) - ro->stack[level].pos - 1) * (ro->bufsize + 8));
      memset(branch->keys + (_rdd_ushort(branch->nkeys) - 1) * (ro->bufsize + 8), 0, ro->bufsize + 8);
      _rdd_put_ushort(branch->nkeys, _rdd_ushort(branch->nkeys) - 1);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	 return er;
      if (level && (ro->stack[level].pos == _rdd_ushort(branch->nkeys)))
      {
	 int s = (_rdd_ushort(branch->nkeys) - 1) * (ro->bufsize + 8);

	 unsigned int rec = _rdd_backuint(branch->keys + s + ro->bufsize);

	 if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1, branch->keys + s, rec, __PROC__)))
	    return er;
      }
   }
   return 0;
}
