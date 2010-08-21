CLIP_DLLEXPORT void
_clip_trap_printf(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *fmt, ...)
{
   va_list ap;

   char buf[256];

   va_start(ap, fmt);
   vsnprintf(buf, sizeof(buf), _clip_gettext(fmt), ap);
   va_end(ap);
   _clip_trap_str(ClipMachineMemory, filename, line, buf);
}
