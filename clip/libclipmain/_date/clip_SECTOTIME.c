int
clip_SECTOTIME(ClipMachine * ClipMachineMemory)
{
   int hour, min, sec, msec, len;

   char *ret;

   double d = _clip_parnd(ClipMachineMemory, 1);

   int flag = _clip_parl(ClipMachineMemory, 2);

   len = 32;			/*flag ? 11 : 8; */
   ret = malloc(len + 1);
   sec = d;
   msec = (d - sec) * 100;
   min = sec / 60;
   hour = min / 60;
   sec = sec % 60;
   min = min % 60;
   if (flag)
      snprintf(ret, len, "%02d:%02d:%02d:%02d", hour, min, sec, msec);
   else
      snprintf(ret, len, "%02d:%02d:%02d", hour, min, sec);
   len = strlen(ret);
   _clip_retcn_m(ClipMachineMemory, ret, len);
   return 0;
}
