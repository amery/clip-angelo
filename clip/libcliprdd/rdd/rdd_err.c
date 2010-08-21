int
rdd_err(ClipMachine * ClipMachineMemory, int genCode, int osCode, const char *subSystem, int line, const char *operation, const char *desc)
{
   ClipVar *vp;

   char buf[256];

   if (osCode)
   {
#ifndef OS_MINGW
      strcpy(ClipMachineMemory->syserr, strerror(osCode));
#else
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), ClipMachineMemory->syserr, sizeof(ClipMachineMemory->syserr), NULL);
      _clip_translate_charset("cp1251", _clip_hostcs, ClipMachineMemory->syserr, ClipMachineMemory->syserr, strlen(ClipMachineMemory->syserr));
#endif
   }
	/*_clip_logg(0,"RDD layer error: %s/%d: %s: %s",subSystem,line,desc,operation);*/
   snprintf(buf, sizeof(buf), "%s: %s", desc, operation);
   vp = _clip_new_error(ClipMachineMemory, _clip_errname(genCode), 2, genCode, osCode, 0, 0, 0, 1, subSystem, line, buf);
   _clip_trap_var(ClipMachineMemory, ClipMachineMemory->fp ? ClipMachineMemory->fp->filename_of_ClipFrame : "", ClipMachineMemory->fp ? ClipMachineMemory->fp->line_of_ClipFrame : 0, vp);
   return genCode;
}
