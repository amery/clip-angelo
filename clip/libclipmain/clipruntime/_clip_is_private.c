CLIP_DLLEXPORT int
_clip_is_private(ClipMachine * ClipMachineMemory, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMachineMemory->privates, hash));

   return vp ? 0 : -1;
}
