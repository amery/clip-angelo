static int
fpt_setvchar(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, int oldlen, unsigned int *id, char *buf, const char *__PROC__)
{
   int oldbls = (oldlen + rm->blocksize - 1) / rm->blocksize;

   int newbls = (len + rm->blocksize - 1) / rm->blocksize;

   char m[4];

   int er;

   if ((*id) && newbls <= oldbls)
   {
      if ((er = rdd_write(ClipMachineMemory, &rm->file, (*id) * rm->blocksize, len, buf, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, 4, m, __PROC__)))
	 return er;
      *id = _rdd_backuint((unsigned char *) m);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, (*id) * rm->blocksize, len, buf, __PROC__)))
	 return er;
      if (len % rm->blocksize)
      {
	 if ((er = rdd_write(ClipMachineMemory, &rm->file, (*id + newbls) * rm->blocksize - 1, 1, "", __PROC__)))
	    return er;
      }
      _rdd_put_backuint((unsigned char *) m, *id + newbls);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, 0, 4, m, __PROC__)))
	 return er;
   }
   return 0;
}
