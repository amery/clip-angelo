int
clip_HASHSTR(ClipMachine * ClipMachineMemory)
{
   char *s;

   long hash;

   char *str = _clip_parc(ClipMachineMemory, 1);

   if (str == NULL)
   {
      _clip_retnl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HASHSTR");
   }
   hash = _clip_hashstr(str);
   _clip_retnl(ClipMachineMemory, hash);

   s = (char *) HashTable_fetch(ClipMachineMemory->hashnames, hash);
   if (!s)
   {
      s = strdup(str);
      HashTable_store(ClipMachineMemory->hashnames, s, hash);
   }
   return 0;
}
