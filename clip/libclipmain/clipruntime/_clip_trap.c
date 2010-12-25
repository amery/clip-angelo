
/* retrapper - add filename&line info into trapStack */
CLIP_DLLEXPORT void
_clip_trap(ClipMachine * ClipMachineMemory, const char *filename, int line)
{
   char *s;

   char buf[256];

   snprintf(buf, 256, "file '%s' line %d", filename, line);
#if 0
   if (!ClipMachineMemory->noerrblock)
      _clip_logg(4, "trace: %s", buf);
#endif
   s = strdup(buf);
   add_ClipVect(&ClipMachineMemory->trapTrace, s);
}
