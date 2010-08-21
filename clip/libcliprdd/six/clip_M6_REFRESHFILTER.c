int
clip_M6_REFRESHFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_REFRESHFILTER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   RDD_FILTER *fp;

   int er;

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
   if (ClipMachineMemory->flags1 & OPTIMIZE_FLAG)
   {
      if (fp->list)
      {
	 free(fp->list);
	 fp->list = NULL;
	 fp->listlen = 0;
      }
      if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err;
      READLOCK;
      if ((er = rdd_initrushmore(ClipMachineMemory, wa->rd, fp, NULL, 0, __PROC__)))
	 goto err_unlock;
      UNLOCK;
   }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
