int
clip_ISLEAP(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   long d1, d2;

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
   d1 = _clip_jdate(1, 1, yy);
   d2 = _clip_jdate(31, 12, yy);
   _clip_retl(ClipMachineMemory, (d2 - d1) == 365);
   return 0;
}
