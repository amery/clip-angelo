int
clip_M6_RECCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_RECCOUNT";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   RDD_FILTER *fp;

   int i, cnt = 0, er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
   {
      er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
      goto err;
   }
   fp = wa->rd->filter;
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (fp->rmap)
   {
      for (i = 1; i <= fp->size; i++)
	 if (_rm_getbit(fp->rmap, fp->size, i))
	    cnt++;
   }

   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err:
   return er;
}
