static int
__flex_prev(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, int *out, int *ok, const char *__PROC__)
{
   FLEX_TREE *parent;

   int er;

   *out = 0;
   *ok = 0;
   tree->pos--;
   if (tree->pos < 0)
   {
      parent = tree->parent;
      if (parent)
      {
	 if ((er = __flex_prev(ClipMachineMemory, rm, parent, out, ok, __PROC__)))
	    return er;
	 if (*out || *ok)
	    return 0;
	 _flex_freetree(tree);
	 if ((er = _flex_last(ClipMachineMemory, rm, parent, __PROC__)))
	    return er;
	 *ok = 1;
      }
      else
      {
	 *out = 1;
      }
   }
   return 0;
}
