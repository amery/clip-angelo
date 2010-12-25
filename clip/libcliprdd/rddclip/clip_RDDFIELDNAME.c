int
clip_RDDFIELDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDNAME";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int fno = _clip_parni(ClipMachineMemory, 2);

   if (!rd)
      return EG_NOTABLE;

   if (fno > rd->nfields)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, rd->fields[fno - 1].name);
   return 0;
}
