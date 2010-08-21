/*
	ULIMIT(cResource [, nNewValue]) -> nValue

	cResource is a one of:
	"CPU"      CPU time in seconds
	"FSIZE"    Maximum filesize
	"DATA"     max data size
	"STACK"    max stack size
	"CORE"     max core file size
	"RSS"      max resident set size
	"NPROC"    max number of processes
	"NOFILE"   max number of open files
	"MEMLOCK"  max locked-in-memory address space
	"AS"       address space (virtual memory) limit
*/
int
clip_ULIMIT(ClipMachine * ClipMachineMemory)
{

#ifdef OS_MINGW
   return EG_ARG;
#else
   char *res = _clip_parc(ClipMachineMemory, 1);

   long newval;

   int resource;

   int r;

   struct rlimit rlimit;

   if (!res)
      return EG_ARG;

#ifdef RLIMIT_CPU
   if (!strcasecmp(res, "CPU"))
      resource = RLIMIT_CPU;
#else
   if (0)
      ;
#endif
#ifdef RLIMIT_FSIZE
   else if (!strcasecmp(res, "FSIZE"))
      resource = RLIMIT_FSIZE;
#endif
#ifdef RLIMIT_DATA
   else if (!strcasecmp(res, "DATA"))
      resource = RLIMIT_DATA;
#endif
#ifdef RLIMIT_STACK
   else if (!strcasecmp(res, "STACK"))
      resource = RLIMIT_STACK;
#endif
#ifdef RLIMIT_CORE
   else if (!strcasecmp(res, "CORE"))
      resource = RLIMIT_CORE;
#endif
#ifdef RLIMIT_RSS
   else if (!strcasecmp(res, "RSS"))
      resource = RLIMIT_RSS;
#endif
#ifdef RLIMIT_NPROC
   else if (!strcasecmp(res, "NPROC"))
      resource = RLIMIT_NPROC;
#endif
#ifdef RLIMIT_NOFILE
   else if (!strcasecmp(res, "NOFILE"))
      resource = RLIMIT_NOFILE;
#endif
#ifdef RLIMIT_MEMLOCK
   else if (!strcasecmp(res, "MEMLOCK"))
      resource = RLIMIT_MEMLOCK;
#endif
#ifdef RLIMIT_AS
   else if (!strcasecmp(res, "AS"))
      resource = RLIMIT_AS;
#else
#ifdef RLIMIT_DATA
   else if (!strcasecmp(res, "AS"))
      resource = RLIMIT_DATA;
#endif
#endif
   else
      return EG_ARG;

   if (ClipMachineMemory->argc > 1)
   {
      newval = _clip_parnl(ClipMachineMemory, 2);
      getrlimit(resource, &rlimit);
      if (newval < 0)
	 rlimit.rlim_cur = RLIM_INFINITY;
      else if (rlimit.rlim_max != RLIM_INFINITY && newval > rlimit.rlim_max)
	 newval = rlimit.rlim_max;
      rlimit.rlim_cur = newval;
      setrlimit(resource, &rlimit);
   }

   r = getrlimit(resource, &rlimit);
   if (r)
      _clip_retnl(ClipMachineMemory, -1);
   else
      _clip_retnl(ClipMachineMemory, rlimit.rlim_cur);

   return 0;
#endif
}
