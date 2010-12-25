int
rdd_continue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *found, const char *__PROC__)
{
   RDD_FILTER *old = NULL;

   int er;

   if (!rd->locate_filter)
      return 0;
   if (rd->filter)
   {
      old = rd->filter;
      old->active = 0;
   }
   rd->filter = rd->locate_filter;
   rd->filter->active = 1;

   if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
      return er;

   if (rd->locate_filter)
      rd->locate_filter->active = 0;
   rd->filter = NULL;
   if (old)
   {
      rd->filter = old;
      rd->filter->active = 1;
   }

   return rdd_locate(ClipMachineMemory, rd, NULL, NULL, NULL, NULL, NULL, NULL, found, __PROC__);
}
