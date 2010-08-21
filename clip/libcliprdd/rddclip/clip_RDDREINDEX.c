int
clip_RDDREINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDREINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      return er;
   return rdd_reindex(ClipMachineMemory, rd, __PROC__);
}
