int
clip_TIMEVALID(ClipMachine * ClipMachineMemory)
{
   int l;

   int hour = 0, min = 0, sec = 0, msec = 0, ret = 1;
   int counts[] = { 0, 0, 0, 0 };
   int count = 0;

   char *s, *e, *str = _clip_parcl(ClipMachineMemory, 1, &l);

   _clip_retl(ClipMachineMemory, 0);
   if (str == NULL)
      return 0;

   for (s = str, e = str + l; s <= e; s++)
   {
      if (*s == ':' || *s == 0 || (*s >= '0' && *s <= '9'))
	 counts[count]++;
      if (*s == ':' || *s == 0)
      {
	 //             printf("\ns=%s,count=%d,c1=%d,c2=%d,c3=%d,c4=%d\n",str,count,counts[0],counts[1],counts[2],counts[3]);
	 if (counts[count] != 0 && counts[count] != 3)
	 {
	    ret = 0;
	    break;
	 }
	 if (*s != 0)
	    count++;
      }
      if (count > 3)
      {
	 ret = 0;
	 break;
      }

   }
   if (ret != 0)
   {
      sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
      if (hour < 0 || hour > 23)
	 ret = 0;
      if (min < 0 || min > 59)
	 ret = 0;
      if (sec < 0 || sec > 59)
	 ret = 0;
      if (msec < 0 || msec > 99)
	 ret = 0;
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
