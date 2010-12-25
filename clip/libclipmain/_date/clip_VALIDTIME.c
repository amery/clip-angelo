int
clip_VALIDTIME(ClipMachine * ClipMachineMemory)
{
   int l, hour = 0, min = 0, sec = 0, msec = 0, ret = 1;

   char *str = _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
   if (hour < 0 || hour > 23)
      ret = 0;
   if (min < 0 || min > 59)
      ret = 0;
   if (sec < 0 || sec > 59)
      ret = 0;
   if (msec < 0 || msec > 99)
      ret = 0;
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
