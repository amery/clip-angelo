int
clip_DOY(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   struct tm *sysTime;

   long d1, d2;

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
   d2 = _clip_jdate(dd, mm, yy);
   _clip_retndp(ClipMachineMemory, d2 - d1, 3, 0);
   return 0;
}
