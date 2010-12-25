static int
_cdx_addkey(ClipMachine * ClipMachineMemory, int area, DbfLocale * loc, ClipVar * v, RDD_ORDER * ro, void *key, int len, unsigned int recno, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int addbytes;

   void *keys = NULL;

   int fr, er;

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   keys = malloc(ro->bufsize * (_rdd_ushort(leaf->nkeys) + 1));
   _cdx_loadkeys(ro, leaf, keys);
   if ((fr = _cdx_leaf2split(ro, leaf, keys, ro->stack[ro->level].pos, key, len, recno, &addbytes)) <= 0)
   {

      CDX_LEAF newleaf;

      unsigned int newpage;

      unsigned int brpage;

      void *key1 = malloc(ro->bufsize);

      void *key2 = malloc(ro->bufsize);

      unsigned int rec1, rec2;

      if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newpage, __PROC__)))
	 goto err;

      if ((er = _cdx_splitleaf(ClipMachineMemory, ro, leaf, &newleaf, ro->stack[ro->level].page, newpage, keys, __PROC__)))
	 goto err;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	 goto err;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, newpage, sizeof(CDX_LEAF), &newleaf, __PROC__)))
	 goto err;

      _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec1, key1);
      _cdx_leaf_item(ro, &newleaf, _rdd_ushort(newleaf.nkeys) - 1, &rec2, key2);
      if (ro->level == 0)
      {
	 if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &brpage, __PROC__)))
	    goto err1;
	 if ((er = _cdx_addroot(ClipMachineMemory, ro, brpage, key1, rec1, ro->stack[ro->level].page, key2, rec2, newpage, __PROC__)))
	    goto err1;
      }
      else
      {
	 brpage = ro->stack[ro->level - 1].page;

	 if ((er = _cdx_addstick(ClipMachineMemory, ro, ro->level - 1, key1, rec1, ro->stack[ro->level].page, key2, rec2, newpage, __PROC__)))
	    goto err1;
      }
      free(key1);
      free(key2);
      if ((er = __cdx_addkey(ClipMachineMemory, area, ro, v, loc, recno, __PROC__)))
	 goto err;
      goto cont;
    err1:
      free(key1);
      free(key2);
      goto err;
   }
   else
   {
      _cdx_addnode(ro, leaf, keys, ro->stack[ro->level].pos, key, len, recno, addbytes, fr);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	 goto err;

      if (ro->level && (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys) - 1))
      {
	 void *key = malloc(ro->bufsize);

	 unsigned int rec;

	 _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec, key);
	 if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, ro->level - 1, key, rec, __PROC__)))
	 {
	    free(key);
	    goto err;
	 }
	 free(key);
      }
   }
 cont:
   free(keys);
   return 0;
 err:
   if (keys)
      free(keys);
   return er;
}
