CLIP_DLLEXPORT void
_clip_store_item_destroy(ClipMachine * ClipMachineMemory, long hash, void *item, void (*destroy) (void *))
{
   item_bucket *ip;

   _clip_free_item(ClipMachineMemory, hash);
   ip = NEW(item_bucket);
   ip->item = item;
   ip->destroy = destroy;
   HashTable_store(ClipMachineMemory->store, ip, hash);
}
