CLIP_DLLEXPORT int
_clip_is_memvar(ClipMachine * ClipMachineMemory, long hash)
{
   return (!_clip_is_private(ClipMachineMemory, hash) || !_clip_is_public(ClipMachineMemory, hash)) ? 0 : 1;
}
