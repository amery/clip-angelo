int
clip_RDDFIELDPOS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDPOS";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   char *fname = _clip_parc(ClipMachineMemory, 2);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname))) + 1);
   return 0;
}
