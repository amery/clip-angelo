CLIP_DLLEXPORT int
_clip_fm_assign(ClipMachine * ClipMachineMemory, long hash)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   int r = _clip_fm_iassign(ClipMachineMemory, hash);

   _clip_destroy(ClipMachineMemory, sp);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return r;
}
