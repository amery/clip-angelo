CLIP_DLLEXPORT int
_clip_array(ClipMachine * ClipMachineMemory, ClipVar * dest, int ndim, long *dims)
{
   new_array(dest, ndim, dims);
   return 0;
}
