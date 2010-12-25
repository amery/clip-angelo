static int
_flex_first(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE];

   int er, i;

   tree->child = calloc(1, sizeof(FLEX_TREE));
   tree->child->parent = tree;
   tree->child->offs = tree->items[tree->pos].page;
   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->child->offs, 4, buf, __PROC__)))
      return er;
   if (_rdd_backuint((unsigned char *) buf) != 0x3e8)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->child->offs + 8, 2, buf, __PROC__)))
      return er;
   tree->child->count = _rdd_ushort((unsigned char *) buf) / 4;
   tree->child->leaf = _rdd_ushort((unsigned char *) buf) % 4 == 3;
   if (tree->child->count > (tree->child->leaf ? FLEX_MAXLEAF : FLEX_MAXPARENT))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   tree->child->items = calloc(tree->child->count, sizeof(FLEX_ITEM));
   if ((er = rdd_read(ClipMachineMemory, &rm->file, tree->child->offs + 10, sizeof(buf), buf, __PROC__)))
      return er;
   for (i = 0; i < tree->child->count; i++)
   {
      if (tree->child->leaf)
      {
	 tree->child->items[i].li_size = _rdd_uint((unsigned char *) (buf + i * 8 + 0));
	 tree->child->items[i].li_offs = _rdd_uint((unsigned char *) (buf + i * 8 + 4));
      }
      else
      {
	 tree->child->items[i].li_size = _rdd_uint((unsigned char *) (buf + i * 12 + 0));
	 tree->child->items[i].li_offs = _rdd_uint((unsigned char *) (buf + i * 12 + 4));
	 tree->child->items[i].page = _rdd_uint((unsigned char *) (buf + i * 12 + 8));
      }
   }
   tree->child->pos = 0;
   if (!tree->child->leaf)
      if ((er = _flex_first(ClipMachineMemory, rm, tree->child, __PROC__)))
	 return er;
   return 0;
}
