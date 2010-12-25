/* trap head of stack */
CLIP_DLLEXPORT void
_clip_trap_pop(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame), *Vp = NEW(ClipVar);

   *Vp = *vp;
   ClipMachineMemory->trapVar = Vp;
}
