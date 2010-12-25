int
_flex_savebranch(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * branch, int size, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE - 10];

   char cb[2];

   int i, er;

   memset(buf, 0xAD, FLEX_MAXPARENT * 12);
   memset(buf + FLEX_MAXPARENT * 12, 0, sizeof(buf) - FLEX_MAXPARENT * 12);
   for (i = 0; i < branch->count; i++)
   {
      _rdd_put_uint((unsigned char *) (buf + i * 12 + (size ? 0 : 4)), branch->items[i].li_size);
      _rdd_put_uint((unsigned char *) (buf + i * 12 + (size ? 4 : 0)), branch->items[i].li_offs);
      _rdd_put_uint((unsigned char *) (buf + i * 12 + 8), branch->items[i].page);
   }
   _rdd_put_ushort((unsigned char *) cb, (short) (branch->count * 4 + 2));
   if ((er = rdd_write(ClipMachineMemory, &rm->file, branch->offs + 8, 2, cb, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rm->file, branch->offs + 10, sizeof(buf), buf, __PROC__)))
      return er;
   return 0;
}
