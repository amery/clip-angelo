int
rm_checkjoin(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   if (!f1 || !f2)
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Invalid handle to filter");
      goto err;
   }
   if (f1->custom != f2->custom)
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join custom and ordinary filter");
      goto err;
   }
   if (f1->rd != f2->rd)
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join different areas filters");
      goto err;
   }
   if ((f1->rmap && !f2->rmap) || (!f1->rmap && f2->rmap))
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join optimized and non-optimized filters");
      goto err;
   }
   if (f1->size != f2->size)
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join filters of different lengths");
      goto err;
   }
   if (!f1->custom)
   {
      if (!f1->sfilter || !f2->sfilter)
      {
	 rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "No expression on filter");
	 goto err;
      }
   }
   return 0;
 err:
   return 1;
}
