CLIP_DLLEXPORT void
_clip_dimarray(ClipMachine * ClipMachineMemory, int n)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   int i;

   long *dims;

#ifdef OS_MINGW
   dims = (long *) malloc(sizeof(long) * n);
#else
   dims = (long *) alloca(sizeof(long) * n);
#endif

   for (i = 0; i < n; i++)
      dims[i] = _clip_long(sp - n + i);

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= n - 1;
   new_array(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1, n, dims);
#ifdef OS_MINGW
   free(dims);
#endif
}
