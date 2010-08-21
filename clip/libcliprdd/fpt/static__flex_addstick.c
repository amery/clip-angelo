static int
_flex_addstick(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * child, unsigned int newpage, FLEX_ITEM * left, FLEX_ITEM * right, int size, const char *__PROC__)
{
   FLEX_TREE *parent = child->parent;

   int er;

   if (!parent)
   {
      FPT_HEADER *hdr = NULL;

      unsigned int newroot;

      char buf[4];

      if ((er = _flex_newpage(ClipMachineMemory, rm, &newroot, 0, __PROC__)))
	 return er;
      _rdd_put_uint((unsigned char *) buf, newroot);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, (long) (size ? &hdr->flexsize : &hdr->flexoffs), 4, buf, __PROC__)))
	 return er;

      parent = (FLEX_TREE *) calloc(1, sizeof(FLEX_TREE));
      parent->root = 1;
      parent->offs = newroot;
      parent->child = child;
      child->parent = parent;
      parent->items = (FLEX_ITEM *) calloc(1, sizeof(FLEX_ITEM));
      parent->count = 1;
   }
   if ((parent->count < 0) || (parent->count > FLEX_MAXPARENT))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   if (parent->count == FLEX_MAXPARENT)
   {
      if ((er = _flex_splitbranch(ClipMachineMemory, rm, parent, size, __PROC__)))
	 return er;
   }
   parent->items = (FLEX_ITEM *) realloc(parent->items, sizeof(FLEX_ITEM) * (parent->count + 1));
   memmove(parent->items + parent->pos + 2, parent->items + parent->pos + 1, 12 * (parent->count - parent->pos - 1));
   parent->items[parent->pos].page = child->offs;
   parent->items[parent->pos].li_offs = left->li_offs;
   parent->items[parent->pos].li_size = left->li_size;
   parent->items[parent->pos + 1].page = newpage;
   parent->items[parent->pos + 1].li_offs = right->li_offs;
   parent->items[parent->pos + 1].li_size = right->li_size;
   parent->count++;
   if ((er = _flex_savebranch(ClipMachineMemory, rm, parent, size, __PROC__)))
      return er;
   return 0;
}
