CLIP_DLLEXPORT void
_clip_free_item(ClipMachine * ClipMachineMemory, long hash)
{
   item_bucket *ip;

   ip = (item_bucket *) HashTable_remove(ClipMachineMemory->store, hash);
   if (ip)
   {
      if (ip->destroy)
	 ip->destroy(ip->item);
      else
	 free(ip->item);
      free(ip);
   }
}
