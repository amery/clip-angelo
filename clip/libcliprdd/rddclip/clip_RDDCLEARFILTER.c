int
clip_RDDCLEARFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLEARFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   return rdd_clearfilter(ClipMachineMemory, rd, __PROC__);
}
