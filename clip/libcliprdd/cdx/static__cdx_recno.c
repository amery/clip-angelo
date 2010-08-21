static int
_cdx_recno(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, unsigned int *recno, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (leaf)
   {
      unsigned long long item;

      unsigned int rec;

      int dup, trail;

      memcpy(&item, leaf->keys + (ro->stack[level].pos) * leaf->bytes, leaf->bytes);
      _CDX_LEAF_ITEM(item, rec, dup, trail);
      *recno = rec;
   }
   else
   {
      *recno = _rdd_backuint(branch->keys + (ro->stack[level].pos + 1) * (ro->bufsize + 8) - 4);
   }
   return 0;
}
