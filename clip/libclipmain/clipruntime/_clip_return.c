CLIP_DLLEXPORT void
_clip_return(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->bp[-ClipMachineMemory->argc - 1] = *(--(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame));
}
