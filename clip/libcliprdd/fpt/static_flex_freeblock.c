static int
flex_freeblock(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int id, const char *__PROC__)
{
   unsigned int offs = id * rm->blocksize;

   char buf[4];

   int len, b, l, er;

   if (id && (rm->format == CLIP_MEMO || rm->format == FLEX_MEMO))
   {
      if ((er = rdd_read(ClipMachineMemory, &rm->file, offs + 4, 4, buf, __PROC__)))
	 return er;
      len = _rdd_backuint((unsigned char *) buf);
      b = (len + 8 + (rm->blocksize - 1)) / rm->blocksize;
      l = b * rm->blocksize;

      if ((er = _flex_freeblock(ClipMachineMemory, rm, offs, l, __PROC__)))
	 return er;
/*
		for(i=0;i<rm->flex_nremoved;i++){
			if((er = _flex_freeblock(ClipMachineMemory,rm,rm->flex_removed[i],FLEX_PAGESIZE,__PROC__)))
				return er;
		}
		free(rm->flex_removed);
		rm->flex_removed = NULL;
		rm->flex_nremoved = 0;
*/
   }
   return 0;
}
