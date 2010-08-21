CLIP_DLLEXPORT int
_clip_unref(ClipMachine * ClipMachineMemory, ClipVar * lval)
{
   _clip_destroy(ClipMachineMemory, lval);
   return 0;
}
