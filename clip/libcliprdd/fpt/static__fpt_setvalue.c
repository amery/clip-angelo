static int
_fpt_setvalue(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int id, int type, char *str, int len, const char *__PROC__)
{
   char buf[4];

   int l = ((len + 8) + rm->blocksize - 1) / rm->blocksize;

   int tl = l * rm->blocksize - (len + 8);

   unsigned int offs = id * rm->blocksize;

   char *tail;

   int er;

   if (!id)
      return 0;
   _rdd_put_backuint((unsigned char *) buf, type);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, offs, 4, buf, __PROC__)))
      return er;
   _rdd_put_backuint((unsigned char *) buf, len);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, offs + 4, 4, buf, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rm->file, offs + 8, len, str, __PROC__)))
      return er;
   if (tl > 0)
   {
      if (tl > 1)
      {
	 tail = calloc(1, tl - 1);
	 if ((er = rdd_write(ClipMachineMemory, &rm->file, offs + 8 + len, tl - 1, tail, __PROC__)))
	 {
	    free(tail);
	    return er;
	 }
	 free(tail);
      }
      buf[0] = 0xAF;
      if ((er = rdd_write(ClipMachineMemory, &rm->file, offs + l * rm->blocksize - 1, 1, buf, __PROC__)))
	 return er;
   }
   return 0;
}
