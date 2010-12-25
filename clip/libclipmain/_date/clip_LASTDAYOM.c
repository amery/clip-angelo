int
clip_LASTDAYOM(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww, mm1;

   struct tm *sysTime;

   long d;

   int numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   mm1 = _clip_parni(ClipMachineMemory, 1);
   if (numpar == 0 || mm1 != 0)
   {
      sysTime = _clip_sysdate();
      yy = sysTime->tm_year + 1900;
      mm = sysTime->tm_mon + 1;
      dd = sysTime->tm_mday;
      free(sysTime);
   }
   if (mm1 != 0)
      mm = mm1;
   d = _clip_jdate(1, mm + 1, yy);
   _clip_cdate(d, &dd, &mm1, &yy, &ww);
   while (mm != mm1)
   {
      d--;
      _clip_cdate(d, &dd, &mm1, &yy, &ww);
   }
   _clip_retndp(ClipMachineMemory, dd, 2, 0);
   return 0;
}
