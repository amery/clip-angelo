int
clip_ADDMONTH(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww, old_mm, i;

   long jdate;

   struct tm *sysTime;

   int numpar = _clip_parinfo(ClipMachineMemory, 0);

   int nummon = _clip_parni(ClipMachineMemory, 1);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
   {
      _clip_retdj(ClipMachineMemory, 0);
      return 0;
   }
   if (numpar == 1)
   {
      sysTime = _clip_sysdate();
      yy = sysTime->tm_year + 1900;
      mm = sysTime->tm_mon + 1;
      dd = sysTime->tm_mday;
      free(sysTime);
      nummon = 1;
   }
   if (numpar > 1)
      nummon = _clip_parni(ClipMachineMemory, 2);
   old_mm = mm + yy * 12;
   mm += nummon;

   if (mm > 12)
   {
      yy += (int) mm / 12;
      mm = mm - ((int) mm / 12) * 12;
   }

   if (nummon != 0)
   {
      for (i = 4; i != 0; i--)
      {
	 jdate = _clip_jdate(dd, mm, yy);
	 _clip_cdate(jdate, &dd, &mm, &yy, &ww);
	 if (((mm + yy * 12) - old_mm) == nummon)
	    break;
	 dd--;
      }
   }
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}
