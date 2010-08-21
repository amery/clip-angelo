CLIP_DLLEXPORT void
_clip_swap(ClipMachine * ClipMachineMemory, int offset)
{
   ClipVar v;

   v = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame[-1];
   memmove(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - offset, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - offset - 1, offset * sizeof(ClipVar));
   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame[-offset - 1] = v;
}
