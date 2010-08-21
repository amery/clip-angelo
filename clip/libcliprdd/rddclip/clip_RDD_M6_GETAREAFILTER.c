int
clip_RDD_M6_GETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_GETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   if (rd->filter)
      _clip_retni(ClipMachineMemory, rd->filter->handle);
   else
      _clip_retni(ClipMachineMemory, -1);
   return 0;
}
