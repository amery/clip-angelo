CLIP_DLLEXPORT int
_clip_push_local(ClipMachine * ClipMachineMemory, int no)
{
   return _clip_push(ClipMachineMemory, local_ref(ClipMachineMemory, no));
}
