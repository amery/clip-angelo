static int
flex_useblock(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int *id, int len, const char *__PROC__)
{
   FLEX_TREE *tree = NULL;

   FLEX_TREE *leaf = NULL;

   char fuu[4];

   int b, l, offs, ll, er;

   if (len == 0)
   {
      *id = 0;
      return 0;
   }

   b = (len + 8 + (rm->blocksize - 1)) / rm->blocksize;
   l = b * rm->blocksize;
   if ((er = _flex_search(ClipMachineMemory, rm, &tree, l, 1, __PROC__)))
      goto err;
   leaf = _flex_leaf(tree);
   if (leaf->pos < leaf->count)
   {
      offs = leaf->items[leaf->pos].li_offs;
      ll = leaf->items[leaf->pos].li_size;
      *id = offs / rm->blocksize;
      if ((er = _flex_remove(ClipMachineMemory, rm, tree, 1, __PROC__)))
	 goto err;
      _flex_freetree(tree);
      if ((er = _flex_search(ClipMachineMemory, rm, &tree, *id * rm->blocksize, 0, __PROC__)))
	 goto err;
      if ((er = _flex_remove(ClipMachineMemory, rm, tree, 0, __PROC__)))
	 goto err;

      if (ll > l)
      {
	 if ((er = _flex_freeblock(ClipMachineMemory, rm, offs + l, ll - l, __PROC__)))
	    return er;
      }
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, 4, fuu, __PROC__)))
	 goto err;
      *id = _rdd_backuint((unsigned char *) fuu);
      _rdd_put_backuint((unsigned char *) fuu, *id + b);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, 0, 4, fuu, __PROC__)))
	 goto err;
   }
   _flex_freetree(tree);
/*
	for(i=0;i<rm->flex_nremoved;i++){
		if((er = _flex_freeblock(ClipMachineMemory,rm,rm->flex_removed[i],FLEX_PAGESIZE,__PROC__)))
			return er;
	}
	free(rm->flex_removed);
	rm->flex_removed = NULL;
	rm->flex_nremoved = 0;
*/
   return 0;
 err:
   _flex_freetree(tree);
   return er;
}
