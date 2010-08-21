CLIP_DLLEXPORT void
_clip_vnewarray(ClipMachine * ClipMachineMemory, int n, long *vect)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   new_array(sp, n, vect);
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
}
