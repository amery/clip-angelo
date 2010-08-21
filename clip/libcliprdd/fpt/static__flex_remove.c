static int
_flex_remove(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, int size, const char *__PROC__)
{
   FLEX_TREE *leaf = _flex_leaf(tree);

   int er;

   if (leaf->count == 1 && leaf->parent)
   {
      rm->flex_removed = realloc(rm->flex_removed, (rm->flex_nremoved + 1) * sizeof(unsigned int));
      rm->flex_removed[rm->flex_nremoved] = leaf->offs;
      rm->flex_nremoved++;

      if ((er = _flex_removestick(ClipMachineMemory, rm, leaf, size, __PROC__)))
	 return er;
   }
   else
   {
      memmove(leaf->items + leaf->pos, leaf->items + leaf->pos + 1, sizeof(FLEX_ITEM) * (leaf->count - leaf->pos - 1));
      leaf->count--;
      leaf->items = (FLEX_ITEM *) realloc(leaf->items, sizeof(FLEX_ITEM) * leaf->count);
      if ((er = _flex_saveleaf(ClipMachineMemory, rm, leaf, size, __PROC__)))
	 return er;
      if (leaf->pos == leaf->count)
      {
	 if ((er = _flex_setlastitem(ClipMachineMemory, rm, leaf->parent, leaf->items + (leaf->count - 1), size, __PROC__)))
	    return er;
      }
   }
   return 0;
}
