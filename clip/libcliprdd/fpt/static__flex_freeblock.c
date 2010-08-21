static int
_flex_freeblock(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, unsigned int offs, int l, const char *__PROC__)
{
   FLEX_TREE *tree = NULL;

   FLEX_TREE *leaf = NULL;

   unsigned int noffs;

   int nl, out, er;

   char buf[4];

   if ((er = _flex_search(ClipMachineMemory, rm, &tree, offs, 0, __PROC__)))
      return er;
   leaf = _flex_leaf(tree);
   if ((leaf->pos < leaf->count) && (offs + l == leaf->items[leaf->pos].li_offs))
   {
      noffs = offs + l;
      nl = leaf->items[leaf->pos].li_size;
      if ((er = _flex_remove(ClipMachineMemory, rm, tree, 0, __PROC__)))
	 return er;
      _flex_freetree(tree);

      if ((er = _flex_exactsearch(ClipMachineMemory, rm, &tree, noffs, nl, __PROC__)))
	 return er;
      if ((er = _flex_remove(ClipMachineMemory, rm, tree, 1, __PROC__)))
	 return er;
      _flex_freetree(tree);

      l += nl;
      if ((er = _flex_search(ClipMachineMemory, rm, &tree, offs, 0, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = _flex_prev(ClipMachineMemory, rm, tree, &out, __PROC__)))
	 return er;
      if (!out)
      {
	 leaf = _flex_leaf(tree);
	 nl = leaf->items[leaf->pos].li_size;
	 noffs = leaf->items[leaf->pos].li_offs;
	 if (noffs + nl == offs)
	 {
	    if ((er = _flex_remove(ClipMachineMemory, rm, tree, 0, __PROC__)))
	       return er;
	    _flex_freetree(tree);

	    if ((er = _flex_exactsearch(ClipMachineMemory, rm, &tree, noffs, nl, __PROC__)))
	       return er;
	    if ((er = _flex_remove(ClipMachineMemory, rm, tree, 1, __PROC__)))
	       return er;

	    offs = noffs;
	    l += nl;
	 }
      }
      _flex_freetree(tree);
      if ((er = _flex_search(ClipMachineMemory, rm, &tree, offs, 0, __PROC__)))
	 return er;
   }

   _rdd_put_backuint((unsigned char *) buf, 0x3e9);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, offs, 4, buf, __PROC__)))
      return er;
   _rdd_put_backuint((unsigned char *) buf, l - 8);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, offs + 4, 4, buf, __PROC__)))
      return er;

   if ((er = _flex_add(ClipMachineMemory, rm, tree, offs, l, 0, __PROC__)))
      return er;
   tree = _flex_root(tree);
   _flex_freetree(tree);

   if ((er = _flex_search(ClipMachineMemory, rm, &tree, l, 1, __PROC__)))
      return er;
   if ((er = _flex_add(ClipMachineMemory, rm, tree, offs, l, 1, __PROC__)))
      return er;
   tree = _flex_root(tree);
   _flex_freetree(tree);

   return 0;
}
