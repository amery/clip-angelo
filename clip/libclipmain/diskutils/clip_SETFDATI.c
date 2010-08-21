int
clip_SETFDATI(ClipMachine * ClipMachineMemory)	/* Sets the date and time of a file */
{
   char *uname = NULL;

   int i = 0, ret = 0;

   struct stat st;

   struct tm t_m, t_a;

   struct utimbuf ut, *u = NULL;

   if (ClipMachineMemory->argc < 1)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
      return 1;
   }
   if ((uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL)
   {
      goto end;
   }

   if (ClipMachineMemory->argc > 1)
   {
      if (stat(uname, &st))
	 goto end;
      t_a = *localtime(&st.st_atime);
      t_m = *localtime(&st.st_mtime);
      for (i = 2; i <= 3; i++)
      {
	 char *ch = NULL;

	 int year = -1, mon = -1, mday = -1, wday = -1;

	 int hr = -1, min = -1, sec = -1;

	 long julian = 0;

	 switch (_clip_parinfo(ClipMachineMemory, i))
	 {
	 case 0:
	    continue;
	 case CHARACTER_type_of_ClipVarType:
	    ch = _clip_parc(ClipMachineMemory, i);
	    sscanf(ch, "%d:%d:%d", &hr, &min, &sec);
	    if (sec < 0)
	       sec = 0;
	    if (min < 0)
	       min = 0;
	    if (hr < 0 || hr > 23 || min > 59 || sec > 59)
	       goto end;
	    t_m.tm_hour = hr;
	    t_m.tm_min = min;
	    t_m.tm_sec = sec;
	    break;
	 case DATE_type_of_ClipVarType:
	    julian = _clip_pardc(ClipMachineMemory, i, &year, &mon, &mday, &wday);
	    t_m.tm_year = year - 1900;
	    t_m.tm_mon = mon - 1;
	    t_m.tm_mday = mday;
	    break;
	 default:
	    goto end;
	 }
      }
      ut.actime = mktime(&t_a);
      ut.modtime = mktime(&t_m);
      u = &ut;
   }

   ret = utime(uname, u) ? 0 : 1;

 end:
   _clip_retl(ClipMachineMemory, ret);

   if (uname != NULL)
      free(uname);
   return 0;
}
