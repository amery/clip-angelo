int
_clip_sprintf(ClipMachine * ClipMachineMemory, const char *fmt, ...)
{
   va_list ap;

   int ret;

   va_start(ap, fmt);
   ret = _clip_vsprintf(ClipMachineMemory, fmt, ap);

   va_end(ap);

   return ret;
}
