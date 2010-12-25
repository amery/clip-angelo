int
clip_CTOT(ClipMachine * ClipMachineMemory)
{
   char *ctime = _clip_parc(ClipMachineMemory, 1);

   long date, time;

   int er;

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOT");

   er = _clip_ctot(ClipMachineMemory, ctime, &date, &time, ClipMachineMemory->date_format);
   if (er)
   {
      _clip_retdtj(ClipMachineMemory, 0, 0);
      return 0;
   }
   _clip_retdtj(ClipMachineMemory, date, time);
   return er;
}
