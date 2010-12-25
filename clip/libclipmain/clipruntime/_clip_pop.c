CLIP_DLLEXPORT void
_clip_pop(ClipMachine * ClipMachineMemory)
{
   _clip_destroy(ClipMachineMemory, --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame));
}
