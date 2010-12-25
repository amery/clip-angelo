static void
delete_spaces(ClipMachine * ClipMachineMemory, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
   {
      VAR(HashTable, vp, HashTable_current(hp));
      delete_VarTable(ClipMachineMemory, vp);
   }
   delete_HashTable(hp);
}
