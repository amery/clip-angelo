int
rdd_clearfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int er;

   if (rd->filter)
   {
      if (rd->filter->active)
      {
	 if ((er = rdd_destroyfilter(ClipMachineMemory, rd->filter, __PROC__)))
	    return er;
      }
      rd->filter = NULL;
   }
   return 0;
}
