int
clip_BOM(ClipMachine * ClipMachineMemory)
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
   _clip_retdc(ClipMachineMemory, yy, mm, 1);
   return 0;
}
