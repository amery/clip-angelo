
CLIP_DLLEXPORT int
_clip_call(struct ClipMachine *ClipMachineMemory, int argc, long hash)
{
   return obj_call(ClipMachineMemory, argc, hash, CALL_CALL);
}
