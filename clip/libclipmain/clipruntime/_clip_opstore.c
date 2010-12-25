CLIP_DLLEXPORT int
_clip_opstore(ClipMachine * ClipMachineMemory, int dim, int op)
{
   ClipVar *vp = clip_fetch(ClipMachineMemory, dim, 0, 1, 0, 0);

   if (!vp)
      return 1;
   _clip_opassign(ClipMachineMemory, vp, op);
   return 0;
}
