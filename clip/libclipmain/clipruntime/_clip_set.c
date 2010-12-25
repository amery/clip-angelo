CLIP_DLLEXPORT int
_clip_set(struct ClipMachine *ClipMachineMemory, long hash)
{
   return obj_call(ClipMachineMemory, 1, hash, CALL_SET);
}
