int
clip_WEEK(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww, week1, week2, ret;

   long d1, d2;

   struct tm *sysTime;

   int numpar = _clip_parinfo(ClipMachineMemory, 0);

   int weekday = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_first_day_week));

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
   week1 = (d1 + weekday) % 7 + 1;
   d2 = _clip_jdate(dd, mm, yy);
   week2 = (d2 + weekday) % 7 + 1;
   ret = (week2 < week1);
   ret = ((d2 - d1) / 7) + ret + 1;
   _clip_retndp(ClipMachineMemory, ret, 2, 0);
   return 0;
}
