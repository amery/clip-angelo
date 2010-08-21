int
_flex_saveleaf(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * leaf, int size, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE - 10];

   char cb[2];

   int i, er;

   memset(buf, 0xAD, FLEX_MAXLEAF * 8);
   memset(buf + FLEX_MAXLEAF * 8, 0, sizeof(buf) - FLEX_MAXLEAF * 8);
   for (i = 0; i < leaf->count; i++)
   {
      _rdd_put_uint((unsigned char *) (buf + i * 8 + (size ? 0 : 4)), leaf->items[i].li_size);
      _rdd_put_uint((unsigned char *) (buf + i * 8 + (size ? 4 : 0)), leaf->items[i].li_offs);
   }
   _rdd_put_ushort((unsigned char *) cb, (short) (leaf->count * 4 + 3));
   if ((er = rdd_write(ClipMachineMemory, &rm->file, leaf->offs + 8, 2, cb, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rm->file, leaf->offs + 10, sizeof(buf), buf, __PROC__)))
      return er;
   return 0;
}
