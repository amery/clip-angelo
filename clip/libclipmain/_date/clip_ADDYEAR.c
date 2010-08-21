int
clip_ADDYEAR(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   struct tm *sysTime;

   int numpar = _clip_parinfo(ClipMachineMemory, 0);

   int numyear = _clip_parni(ClipMachineMemory, 1);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
   {
      _clip_retdj(ClipMachineMemory, 0);
      return 0;
   }
   if (numpar == 1)
   {
      sysTime = _clip_sysdate();
      yy = sysTime->tm_year + 1901;
      mm = sysTime->tm_mon + 1;
      dd = sysTime->tm_mday;
      free(sysTime);
   }
   if (numpar > 1)
      numyear = _clip_parni(ClipMachineMemory, 2);
   yy += numyear;
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}
