static int
__flex_search(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, int l, int size, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE - 10];

   int i, er;

   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->offs, 4, buf, __PROC__)))
      return er;
   if (_rdd_backuint((unsigned char *) buf) != 0x3e8)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->offs + 8, 2, buf, __PROC__)))
      return er;
   tree->count = _rdd_ushort((unsigned char *) buf) / 4;
   tree->leaf = _rdd_ushort((unsigned char *) buf) % 4 == 3;
   if (tree->count > (tree->leaf ? FLEX_MAXLEAF : FLEX_MAXPARENT))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   tree->items = calloc(tree->count, sizeof(FLEX_ITEM));
   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->offs + 10, sizeof(buf), buf, __PROC__)))
      return er;
   for (i = 0; i < tree->count; i++)
   {
      if (tree->leaf)
      {
	 tree->items[i].li_size = _rdd_uint((unsigned char *) (buf + i * 8 + (size ? 0 : 4)));
	 tree->items[i].li_offs = _rdd_uint((unsigned char *) (buf + i * 8 + (size ? 4 : 0)));
      }
      else
      {
	 tree->items[i].li_size = _rdd_uint((unsigned char *) (buf + i * 12 + (size ? 0 : 4)));
	 tree->items[i].li_offs = _rdd_uint((unsigned char *) (buf + i * 12 + (size ? 4 : 0)));
	 tree->items[i].page = _rdd_uint((unsigned char *) (buf + i * 12 + 8));
      }
   }
   tree->pos = 0;
   if (size)
   {
      while ((tree->items[tree->pos].li_size < l) && (tree->pos < tree->count - !tree->leaf))
	 tree->pos++;
   }
   else
   {
      while ((tree->items[tree->pos].li_offs < l) && (tree->pos < tree->count - !tree->leaf))
	 tree->pos++;
   }
   if (!tree->leaf)
   {
      tree->child = calloc(1, sizeof(FLEX_TREE));
      tree->child->offs = tree->items[tree->pos].page;
      tree->child->parent = tree;
      if ((er = __flex_search(ClipMachineMemory, rm, tree->child, l, size, __PROC__)))
	 return er;
   }
   return 0;
}
