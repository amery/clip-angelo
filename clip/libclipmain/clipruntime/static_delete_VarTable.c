static void
delete_VarTable(ClipMachine * ClipMachineMemory, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
   {
      VAR(VarEntry, vp, HashTable_current(hp));
      delete_VarEntry(ClipMachineMemory, vp);
   }
   delete_HashTable(hp);
}
