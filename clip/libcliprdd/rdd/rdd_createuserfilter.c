int
rdd_createuserfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER ** fpp, unsigned int size, const char *__PROC__)
{
   RDD_FILTER *fp = calloc(sizeof(RDD_FILTER), 1);

   int bytes;

   int er;

   fp->rd = rd;
   if (size)
   {
      bytes = ((size + 1) >> 5) + 1;
      fp->size = size;
   }
   else
   {
      int lastrec;

      if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	 goto err;
      bytes = ((lastrec + 1) >> 5) + 1;
      fp->size = lastrec;
   }
   fp->rmap = calloc(sizeof(unsigned int), bytes);
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   fp->custom = 1;
   *fpp = fp;
   return 0;
 err:
   free(fp);
   return er;
}
