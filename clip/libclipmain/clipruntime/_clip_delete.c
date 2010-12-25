CLIP_DLLEXPORT void
_clip_delete(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   if (!vp)
      return;
   _clip_destroy(ClipMachineMemory, vp);
   free(vp);
}
