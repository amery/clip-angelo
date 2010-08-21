static int
_flex_setlastitem(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * branch, FLEX_ITEM * item, int size, const char *__PROC__)
{
   FLEX_ITEM *ci;

   int er;

   if (!branch)
      return 0;
   ci = branch->items + branch->pos;
   ci->li_offs = item->li_offs;
   ci->li_size = item->li_size;
   if ((er = _flex_savebranch(ClipMachineMemory, rm, branch, size, __PROC__)))
      return er;
   if (branch->pos == branch->count - 1)
   {
      if ((er = _flex_setlastitem(ClipMachineMemory, rm, branch->parent, item, size, __PROC__)))
	 return er;
   }
   return er;
}
