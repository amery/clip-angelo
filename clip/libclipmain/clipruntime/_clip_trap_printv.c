CLIP_DLLEXPORT void
_clip_trap_printv(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *fmt, void *ap)
{
   char buf[256];

   vsnprintf(buf, sizeof(buf), _clip_gettext(fmt), ap);
   _clip_trap_str(ClipMachineMemory, filename, line, buf);
}
