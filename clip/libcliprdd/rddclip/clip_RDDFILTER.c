int
clip_RDDFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retc(ClipMachineMemory, "");
   if (rd->filter && rd->filter->sfilter)
      _clip_retc(ClipMachineMemory, rd->filter->sfilter);
   return 0;
}
