static int
_cdx_savebtree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, BTREE1 * bt, int lastrec, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE page;

   CDX_LEAF *leaf = (CDX_LEAF *) & page;

   int eof, er;

   void *pos = (void *) leaf + sizeof(CDX_LEAF);

   void *buf = calloc(1, ro->bufsize);

   ro->level = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[ro->level].page = _rdd_uint(hdr.root);
   eof = bt1_first(bt);
   _cdx_init_leaf(ro, leaf, lastrec, 0xffffffff);
   while (!eof)
   {
      while (_cdx_add(ro, leaf, bt1_key(bt), &pos, buf) > ro->bufsize + leaf->bytes)
      {
	 eof = bt1_next(bt);
	 if (eof)
	    break;
      }
      if (_rdd_ushort(leaf->attrs) == 3 && eof)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	    goto err;
	 break;
      }
      _rdd_put_ushort(leaf->attrs, 2);
      if (eof)
	 bt1_last(bt);
      pos = (void *) leaf + sizeof(CDX_LEAF);
      if (_rdd_uint(leaf->left) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(leaf->left) + 8, 4, &ro->stack[ro->level].page, __PROC__)))
	    goto err;
      }
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	 goto err;
      _cdx_init_leaf(ro, leaf, lastrec, ro->stack[ro->level].page);
      if ((er = _cdx_adds(ClipMachineMemory, ro, bt1_key(bt), __PROC__)))
	 goto err;
      eof = bt1_next(bt);
      if (eof)
	 break;
      if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &ro->stack[ro->level].page, __PROC__)))
	 goto err;
   }
   free(buf);
   return 0;
 err:
   free(buf);
   return er;
}
