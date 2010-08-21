int
clip_QUARTER(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   struct tm *sysTime;

   int numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
   {
      sysTime = _clip_sysdate();
      yy = sysTime->tm_year + 1900;
      mm = sysTime->tm_mon + 1;
      dd = sysTime->tm_mday;
      free(sysTime);
   }
   _clip_retndp(ClipMachineMemory, mm / 4 + 1, 2, 0);
   return 0;
}
