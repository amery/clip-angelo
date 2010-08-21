CLIP_DLLEXPORT long
_clip_casehash(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   return calc_casehash(ClipMachineMemory, vp, 0);
}
