static int
_flex_add(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, unsigned int offs, int l, int size, const char *__PROC__)
{
   FLEX_TREE *leaf = _flex_leaf(tree);

   int er;

   if ((leaf->count < 0) || (leaf->count > FLEX_MAXLEAF))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   if (leaf->count == FLEX_MAXLEAF)
   {
      if ((er = _flex_splitleaf(ClipMachineMemory, rm, leaf, size, __PROC__)))
	 return er;
   }
   leaf->items = realloc(leaf->items, sizeof(FLEX_ITEM) * (leaf->count + 1));
   memmove(leaf->items + leaf->pos + 1, leaf->items + leaf->pos, sizeof(FLEX_ITEM) * (leaf->count - leaf->pos));
   leaf->items[leaf->pos].li_offs = offs;
   leaf->items[leaf->pos].li_size = l;
   leaf->count++;
   if ((er = _flex_saveleaf(ClipMachineMemory, rm, leaf, size, __PROC__)))
      return er;
   if (leaf->pos == leaf->count - 1)
   {
      if ((er = _flex_setlastitem(ClipMachineMemory, rm, leaf->parent, leaf->items + (leaf->count - 1), size, __PROC__)))
	 return er;
   }
   return 0;
}
