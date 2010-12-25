static int
_flex_newpage(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, unsigned int *page, int leaf, const char *__PROC__)
{
   char buf[FLEX_PAGESIZE - 10];

   int er;

   int s;

   if (rm->flex_nremoved)
   {
      rm->flex_nremoved--;
      *page = rm->flex_removed[rm->flex_nremoved];
      rm->flex_removed = (unsigned int *) realloc(rm->flex_removed, sizeof(int) * rm->flex_nremoved);
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, 4, buf, __PROC__)))
	 return er;
      *page = _rdd_backuint((unsigned char *) buf) * rm->blocksize;
      _rdd_put_backuint((unsigned char *) buf, _rdd_backuint((unsigned char *) buf) + (FLEX_PAGESIZE / rm->blocksize));
      if ((er = rdd_write(ClipMachineMemory, &rm->file, 0, 4, buf, __PROC__)))
	 return er;
   }
   _rdd_put_backuint((unsigned char *) buf, 0x3e8);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, *page, 4, buf, __PROC__)))
      return er;
   if (leaf)
      s = FLEX_MAXLEAF * 8;
   else
      s = FLEX_MAXPARENT * 12;
   _rdd_put_backuint((unsigned char *) buf, s + 2);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, *page + 4, 4, buf, __PROC__)))
      return er;
   _rdd_put_ushort((unsigned char *) buf, leaf ? 3 : 2);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, *page + 8, 2, buf, __PROC__)))
      return er;
   memset(buf, 0xad, s);
   memset(buf + s, 0, FLEX_PAGESIZE - 10 - s);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, *page + 10, sizeof(buf), buf, __PROC__)))
      return er;
   return 0;
}
