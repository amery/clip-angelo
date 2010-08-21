static int
_cdx_recno_keyval(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, unsigned int *recno, void *keyval, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (leaf)
   {
      _cdx_leaf_item(ro, leaf, ro->stack[level].pos, recno, keyval);
   }
   else
   {
   }
   return 0;
}
