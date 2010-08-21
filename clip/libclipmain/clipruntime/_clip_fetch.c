CLIP_DLLEXPORT int
_clip_fetch(ClipMachine * ClipMachineMemory, int dim)
{
   return clip_fetch(ClipMachineMemory, dim, 0, 0, 0, 0) ? 0 : 1;
}
