int
clip_RDDFCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFCOUNT";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, rd->nfields);
   return 0;
}
