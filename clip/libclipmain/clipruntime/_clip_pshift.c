CLIP_DLLEXPORT void
_clip_pshift(ClipMachine * ClipMachineMemory)
{
   --ClipMachineMemory->argc;
   _clip_pcount(ClipMachineMemory);
}
