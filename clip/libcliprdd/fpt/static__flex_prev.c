static int
_flex_prev(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, int *out, const char *__PROC__)
{
   int ok;

   tree = _flex_leaf(tree);
   return __flex_prev(ClipMachineMemory, rm, tree, out, &ok, __PROC__);
}
