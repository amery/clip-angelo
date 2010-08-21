CLIP_DLLEXPORT int
_clip_is_public(ClipMachine * ClipMachineMemory, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMachineMemory->publics, hash));

   return vp ? 0 : -1;
}
