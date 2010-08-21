int
clip_RDDCLEARINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLEARINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   return rdd_clearindex(ClipMachineMemory, rd, __PROC__);
}
