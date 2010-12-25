CLIP_DLLEXPORT void *
_clip_fetch_item(ClipMachine * ClipMachineMemory, long hash)
{
   item_bucket *ip;

   ip = (item_bucket *) HashTable_fetch(ClipMachineMemory->store, hash);
   if (ip)
      return ip->item;
   else
      return 0;
}
