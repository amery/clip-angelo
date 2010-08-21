CLIP_DLLEXPORT void
_clip_store_item(ClipMachine * ClipMachineMemory, long hash, void *item)
{
   item_bucket *ip;

   _clip_free_item(ClipMachineMemory, hash);
   ip = NEW(item_bucket);
   ip->item = item;
   HashTable_store(ClipMachineMemory->store, ip, hash);
}
