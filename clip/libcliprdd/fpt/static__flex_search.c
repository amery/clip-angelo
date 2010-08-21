static int
_flex_search(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE ** ptree, int l, int size, const char *__PROC__)
{
   FPT_HEADER hdr;

   FLEX_TREE *tree;

   unsigned int rootpage;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, sizeof(FPT_HEADER), &hdr, __PROC__)))
      return 0;
   tree = calloc(1, sizeof(FLEX_TREE));
   tree->root = 1;
   tree->offs = _rdd_uint((unsigned char *) (size ? hdr.flexsize : hdr.flexoffs));
   if (!tree->offs)
   {
      if ((er = _flex_newpage(ClipMachineMemory, rm, &rootpage, 1, __PROC__)))
	 goto err;
      _rdd_put_uint((unsigned char *) (size ? hdr.flexsize : hdr.flexoffs), rootpage);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, hdr.flexsize - hdr.fuu, 8, &hdr.flexsize, __PROC__)))
	 goto err;
      tree->leaf = 1;
      tree->offs = rootpage;
   }
   else
   {
      if ((er = __flex_search(ClipMachineMemory, rm, tree, l, size, __PROC__)))
	 goto err;
   }
   *ptree = tree;
   return er;
 err:
   _flex_freetree(tree);
   return er;
}
