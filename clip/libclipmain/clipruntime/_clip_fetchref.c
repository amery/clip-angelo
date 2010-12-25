CLIP_DLLEXPORT ClipVar *
_clip_fetchref(ClipMachine * ClipMachineMemory, int dim)
{
   ClipVar *vp;

   vp = clip_fetch(ClipMachineMemory, dim, 0, 0, 0, 0);
   /*printf("%p\n",vp); */
   return vp;
}
