CLIP_DLLEXPORT void
_clip_retc(ClipMachine * ClipMachineMemory, char *s)
{
   int l = 0;

   if (s)
      l = strlen(s);
   _clip_retcn(ClipMachineMemory, s, l);
}
