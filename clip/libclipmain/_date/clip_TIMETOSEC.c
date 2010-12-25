int
clip_TIMETOSEC(ClipMachine * ClipMachineMemory)
{
   int l, hour = 0, min = 0, sec = 0, msec = 0;

   double ret;

   char *str = _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
   {
      _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
      return 0;
   }
   sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
   ret = hour * 3600 + min * 60 + sec;
   ret = (ret * 100 + msec) / 100;
   _clip_retndp(ClipMachineMemory, ret, 10, 2);
   return 0;
}
