int
clip_ARRAY(ClipMachine * ClipMachineMemory)
{
   int i, ndim, r;

   long *dims;

   ClipVar *rp;

   rp = RETPTR(ClipMachineMemory);

   ndim = ClipMachineMemory->argc;
#ifdef OS_MINGW
   dims = (long *) malloc(ndim * sizeof(long));
#else
   dims = (long *) alloca(ndim * sizeof(long));
#endif

   for (i = 1; i <= ndim; ++i)
      dims[i - 1] = _clip_parni(ClipMachineMemory, i);

   r = _clip_array(ClipMachineMemory, rp, ndim, dims);

   /*decount(rp); */

#ifdef OS_MINGW
   free(dims);
#endif
   return r;
}
