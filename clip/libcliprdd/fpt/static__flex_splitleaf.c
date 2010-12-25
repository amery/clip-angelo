static int
_flex_splitleaf(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * leaf, int size, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE];

   char newbuf[FLEX_PAGESIZE];

   int half = leaf->count / 2;

   unsigned int newpage;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rm->file, leaf->offs, FLEX_PAGESIZE, buf, __PROC__)))
      return er;
   if (_rdd_backuint((unsigned char *) buf) != 0x3e8)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   _rdd_put_ushort((unsigned char *) (newbuf + 8), (short) ((leaf->count - half) * 4 + 3));
   memset(newbuf + 10, 0, FLEX_PAGESIZE - 10);
   memset(newbuf + 10, 0xAD, FLEX_MAXLEAF * 8);
   memcpy(newbuf + 10, buf + 10 + half * 8, (leaf->count - half) * 8);
   if ((er = _flex_newpage(ClipMachineMemory, rm, &newpage, 1, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rm->file, newpage + 8, FLEX_PAGESIZE - 8, newbuf + 8, __PROC__)))
      return er;

   _rdd_put_ushort((unsigned char *) (buf + 8), (short) (half * 4 + 3));
   memset(buf + 10 + half * 8, 0xAD, (leaf->count - half) * 8);
   memset(buf + 10 + leaf->count * 8, 0, FLEX_PAGESIZE - (10 + leaf->count * 8));
   if ((er = rdd_write(ClipMachineMemory, &rm->file, leaf->offs + 8, FLEX_PAGESIZE - 8, buf + 8, __PROC__)))
      return er;

   if ((er = _flex_addstick(ClipMachineMemory, rm, leaf, newpage, leaf->items + (half - 1), leaf->items + (leaf->count - 1), size, __PROC__)))
      return er;
   if (leaf->pos < half)
   {
      leaf->items = (FLEX_ITEM *) realloc(leaf->items, sizeof(FLEX_ITEM) * half);
      leaf->count = half;
   }
   else
   {
      leaf->pos -= half;
      memmove(leaf->items, leaf->items + half, sizeof(FLEX_ITEM) * (leaf->count - half));
      leaf->count -= half;
      leaf->offs = newpage;
      leaf->parent->pos++;
   }
   leaf->root = 0;
   return 0;
}
