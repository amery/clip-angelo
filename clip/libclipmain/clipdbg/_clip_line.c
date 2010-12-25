CLIP_DLLEXPORT int
_clip_line(ClipMachine * ClipMachineMemory)
{
   if (!_clip_debuglevel)
      return 0;
   _clip_debug(ClipMachineMemory);
   return 0;
}
