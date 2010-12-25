/* create with n dimentions */
CLIP_DLLEXPORT void
_clip_newarray(ClipMachine * ClipMachineMemory, int n, ...)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   va_list ap;

   va_start(ap, n);
   new_array(sp, n, (long *) ap);
   va_end(ap);

   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
}
