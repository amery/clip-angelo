CLIP_DLLEXPORT struct _HashTable *
_clip_space(ClipMachine * ClipMachineMemory, long space)
{
   VAR(HashTable, hp, HashTable_fetch(ClipMachineMemory->spaces, space));
   if (!hp)
   {
      hp = new_HashTable();
      HashTable_insert(ClipMachineMemory->spaces, hp, space);
   }
   return hp;
}
