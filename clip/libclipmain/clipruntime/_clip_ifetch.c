CLIP_DLLEXPORT int
_clip_ifetch(ClipMachine * ClipMachineMemory, int dim)
{
   return clip_fetch(ClipMachineMemory, dim, 1, 0, 0, 0) ? 0 : 1;
}
