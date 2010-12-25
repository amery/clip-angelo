static void
clear_VarTable(ClipMachine * ClipMachineMemory, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
   {
      VAR(VarEntry, vp, HashTable_current(hp));
      delete_VarEntry(ClipMachineMemory, vp);
   }
   HashTable_clear(hp);
}
