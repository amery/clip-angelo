CLIP_DLLEXPORT int
_clip_get(struct ClipMachine *ClipMachineMemory, long hash)
{
   return obj_call(ClipMachineMemory, 0, hash, CALL_GET);
}
