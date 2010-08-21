static int
_flex_splitbranch(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * branch, int size, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE];

   char newbuf[FLEX_PAGESIZE];

   int half = branch->count / 2;

   unsigned int newpage;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rm->file, branch->offs, FLEX_PAGESIZE, buf, __PROC__)))
      return er;
   if (_rdd_backuint((unsigned char *) buf) != 0x3e8)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   _rdd_put_ushort((unsigned char *) (newbuf + 8), (short) ((branch->count - half) * 4 + 2));
   memset(newbuf + 10, 0, FLEX_PAGESIZE - 10);
   memset(newbuf + 10, 0xAD, FLEX_MAXPARENT * 12);
   memcpy(newbuf + 10, buf + 10 + half * 12, (branch->count - half) * 12);
   if ((er = _flex_newpage(ClipMachineMemory, rm, &newpage, 0, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rm->file, newpage + 8, FLEX_PAGESIZE - 8, newbuf + 8, __PROC__)))
      return er;

   _rdd_put_ushort((unsigned char *) (buf + 8), (short) (half * 4 + 2));
   memset(buf + 10 + half * 12, 0xAD, (branch->count - half) * 12);
   memset(buf + 10 + branch->count * 12, 0, FLEX_PAGESIZE - (10 + branch->count * 12));
   if ((er = rdd_write(ClipMachineMemory, &rm->file, branch->offs + 8, FLEX_PAGESIZE - 8, buf + 8, __PROC__)))
      return er;

   if ((er = _flex_addstick(ClipMachineMemory, rm, branch, newpage, branch->items + (half - 1), branch->items + (branch->count - 1), size, __PROC__)))
      return er;
   if (branch->pos < half)
   {
      branch->items = (FLEX_ITEM *) realloc(branch->items, sizeof(FLEX_ITEM) * half);
      branch->count = half;
   }
   else
   {
      branch->pos -= half;
      memmove(branch->items, branch->items + half, sizeof(FLEX_ITEM) * (branch->count - half));
      branch->count -= half;
      branch->offs = newpage;
      branch->parent->pos++;
   }
   branch->root = 0;
   return 0;
}
