int
clip_DTOJ(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      struct tm *sysTime;

      sysTime = _clip_sysdate();
      _clip_retndp(ClipMachineMemory, _clip_jdate(sysTime->tm_mday, sysTime->tm_mon + 1, sysTime->tm_year + 1900), 7, 0);
      free(sysTime);
      return 0;
   }

   _clip_retndp(ClipMachineMemory, _clip_pardj(ClipMachineMemory, 1), 7, 0);
   return 0;
}
