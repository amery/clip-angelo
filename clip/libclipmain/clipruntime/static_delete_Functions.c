static void
delete_Functions(ClipMachine * ClipMachineMemory, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
   {
      VAR(FuncEntry, fp, HashTable_current(hp));
      /*if (!fp->f)
         delete_ClipFile(ClipMachineMemory, fp->block.file); */
      free(fp);
   }
   delete_HashTable(hp);
}
