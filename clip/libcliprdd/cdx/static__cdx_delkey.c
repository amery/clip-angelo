static int
_cdx_delkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if ((_rdd_ushort(leaf->nkeys) == 1) && (ro->level > 0))
   {
      if (_rdd_uint(leaf->left) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(leaf->left) + 8, 4, leaf->right, __PROC__)))
	    return er;
      }
      if (_rdd_uint(leaf->right) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(leaf->right) + 4, 4, leaf->left, __PROC__)))
	    return er;
      }
      if ((er = _cdx_putfreepage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
	 return er;
      if ((er = _cdx_delstick(ClipMachineMemory, ro, ro->level - 1, __PROC__)))
	 return er;
   }
   else
   {
      _cdx_delnode(ro, leaf, ro->stack[ro->level].pos);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	 return er;
      if (ro->level && (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys)))
      {
	 void *key = malloc(ro->bufsize);

	 unsigned int rec;

	 _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec, key);
	 if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, ro->level - 1, key, rec, __PROC__)))
	 {
	    free(key);
	    return er;
	 }
	 free(key);
      }
   }
   return 0;
}
