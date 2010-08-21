static int
_flex_exactsearch(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE ** ptree, unsigned int noffs, int nl, const char *__PROC__)
{
   FLEX_TREE *leaf;

   int out = 0, er;

   if ((er = _flex_search(ClipMachineMemory, rm, ptree, nl, 1, __PROC__)))
      return er;
   leaf = _flex_leaf(*ptree);
   while ((leaf->pos < leaf->count) && (leaf->items[leaf->pos].li_offs != noffs))
   {
      if ((er = _flex_next(ClipMachineMemory, rm, *ptree, &out, __PROC__)))
	 return er;
      leaf = _flex_leaf(*ptree);
   }
   if (out)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   return 0;
}
