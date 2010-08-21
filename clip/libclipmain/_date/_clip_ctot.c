int
_clip_ctot(ClipMachine * ClipMachineMemory, char *ctime, long *date, long *time, char *dateformat)
{
   char *s = ctime;

   *date = 0;
   *time = 0;

   if (!ctime)
      return 1;

   while (*s == ' ')
      s++;
//      if(strlen(s) < 5) /* 11:12 */
//              return 1;

   if (s[2] == ':' || strlen(s) < 5)
   {
      *date = 0 /*2415019 */ ;
      *time = _clip_str_to_time(s);
      if (*time == -1)
	 return 1;
   }
   else
   {
      char *t = strchr(s, ' ');

      *date = _clip_str_to_date(s, dateformat, ClipMachineMemory->epoch);
      if (*date == 0)
	 return 1;
      *time = _clip_str_to_time(t);
      if (*time == -1)
	 return 1;
   }
   return 0;
}
