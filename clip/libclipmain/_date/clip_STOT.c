int
clip_STOT(ClipMachine * ClipMachineMemory)
{
   char *ctime = _clip_parc(ClipMachineMemory, 1);

   long date, time;

   int er;

   er = _clip_ctot(ClipMachineMemory, ctime, &date, &time, "yyyy-mm-dd");
   if (er)
   {
      _clip_retdtj(ClipMachineMemory, 0, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOT");
   }
   _clip_retdtj(ClipMachineMemory, date, time);
   return er;
}
