int
_clip_vsprintf(ClipMachine * ClipMachineMemory, const char *fmt, void *ap)
{
   for (;;)
   {
      int n = vsnprintf(ClipMachineMemory->buf, ClipMachineMemory->buflen, fmt, ap);

      if (n < ClipMachineMemory->buflen)
	 return n;
      ClipMachineMemory->buflen = ClipMachineMemory->buflen * 2;
      ClipMachineMemory->buf = (char *) realloc(ClipMachineMemory->buf, ClipMachineMemory->buflen);
   }
}
