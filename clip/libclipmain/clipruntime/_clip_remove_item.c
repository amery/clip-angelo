CLIP_DLLEXPORT void
_clip_remove_item(ClipMachine * ClipMachineMemory, long hash)
{
   void *rp;

   rp = HashTable_remove(ClipMachineMemory->store, hash);
   free(rp);
}
