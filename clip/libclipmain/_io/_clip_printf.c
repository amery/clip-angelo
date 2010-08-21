int
_clip_printf(ClipMachine * ClipMachineMemory, const char *fmt, ...)
{
   int n;

   va_list ap;

   va_start(ap, fmt);
   n = _clip_vsprintf(ClipMachineMemory, fmt, ap);
   out_dev(ClipMachineMemory, ClipMachineMemory->buf, n, _clip_colorSelect(ClipMachineMemory), 1);
   va_end(ap);

   return n;
}
