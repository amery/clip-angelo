/*
	dbgconnect(cProgramNname|nPid) -> lResult
*/
int
clip_DBGCONNECT(ClipMachine * ClipMachineMemory)
{
#ifdef _WIN32
   return EG_ARG;
#else
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   pid_t dpid = 0;

   if (!vp)
      return EG_ARG;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      dpid = _clip_double(vp);
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      char path[256];

      FILE *file;

      char *s = _clip_parc(ClipMachineMemory, 1);

      if (!s || !*s)
	 return EG_ARG;

      snprintf(path, sizeof(path), "pidof -s %s", s);
      file = popen(path, "r");
      if (file)
      {
	 unsigned long ul;

	 if (fscanf(file, "%lu", &ul) == 1)
	    dpid = ul;
	 pclose(file);
      }
   }

   if (!dpid)
      return EG_ARG;

   pid = dpid;

   if (!cleanup_installed)
   {
      cleanup_installed = 1;
      signal(SIGPIPE, dbg_signal);
      atexit(cleanup);
   }

   snprintf(in_path, sizeof(in_path), "/tmp/clip_dbg.%lu.in", (unsigned long) pid);
   remove(in_path);
   if (mkfifo(in_path, 0600))
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "DBGCONNECT: cannot create FIFO '%s': %s\n", in_path, strerror(errno));
      cleanup();
      return -1;
   }

   _clip_logg(1, "fifo %s created successfully", in_path);

   snprintf(out_path, sizeof(out_path), "/tmp/clip_dbg.%lu.out", (unsigned long) pid);
   remove(out_path);
   if (mkfifo(out_path, 0600))
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "DBGCONNECT: cannot create FIFO '%s': %s\n", out_path, strerror(errno));
      cleanup();
      return -1;
   }
   _clip_logg(1, "fifo %s created successfully", out_path);

   if (kill(pid, SIGUSR1))
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "DBGCONNECT: cannot send signal SIGUSR1 to pid %lu: %s\n", (unsigned long) pid, strerror(errno));
      cleanup();
      return -1;
   }
   _clip_logg(1, "signal SIGUSR1 sent to pid %lu", (unsigned long) pid);

   fin = fopen(in_path, "wt");
   if (!fin)
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCONNECT: cannot open FIFO '%s': %s\n", in_path, strerror(errno));
      cleanup();
      return -1;
   }
   setvbuf(fin, 0, _IOLBF, 0);
   fwrite("\n", 1, 1, fin);
   _clip_logg(1, "fifo %s opened for writing", in_path);

   fout = fopen(out_path, "rt");
   if (!fout)
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "DBGCONNECT: cannot open FIFO '%s': %s\n", out_path, strerror(errno));
      cleanup();
      return -1;
   }
   /*setvbuf(fout, 0, _IOLBF, 0); */

   _clip_logg(1, "fifo %s opened for reading", out_path);
   _clip_retl(ClipMachineMemory, 1);

   for (;;)
   {
      char buf[4096];

      if (!fgets(buf, 4096, fout))
      {
	 _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	 cleanup();
	 return -1;
      }
      if (!strcmp(buf, ".\n"))
	 break;
   }

   return 0;
#endif
}
