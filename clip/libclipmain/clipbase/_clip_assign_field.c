CLIP_DLLEXPORT int
_clip_assign_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   int r = _clip_iassign_field(ClipMachineMemory, hash, area);

   _clip_destroy(ClipMachineMemory, sp);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return r;
}
