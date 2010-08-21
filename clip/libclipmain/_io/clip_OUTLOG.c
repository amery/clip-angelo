int
clip_OUTLOG(ClipMachine * ClipMachineMemory)
{
   char buf[11];

   int i;

   time_t tt;

   struct tm *tp;

   int level = _clip_parni(ClipMachineMemory, 1);

   if (level > log_level)
      return 0;

   tt = time(0);
   tp = localtime(&tt);
   snprintf(buf, 11, "%02d:%02d:%02d: ", tp->tm_hour, tp->tm_min, tp->tm_sec);
   _clip_out_log(buf, 10);
   buf[0] = ',';
   buf[1] = 0;
   for (i = 1; i <= ClipMachineMemory->argc; ++i)
   {
      ClipVar *vp = _clip_par(ClipMachineMemory, i);

      if (i > 1)
	 out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_LOG);
      print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_LOG, 0);
      if (i < ClipMachineMemory->argc)
	 _clip_out_log(buf, 1);
   }
   buf[0] = '\n';

   _clip_out_log(buf, 1);
   _clip_flush_log();
   return 0;
}
