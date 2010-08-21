int
_clip_register_block(ClipMachine * ClipMachineMemory, ClipBlock block, long hash)
{
   NEWVAR(FuncEntry, fp);
   fp->ClipFunction_f_of_FuncEntry = 0;
   fp->ClipBlock_block_of_FuncEntry = block;
   fp->hash_of_FuncEntry = hash;
   /*HashTable_remove(ClipMachineMemory->functions, hash); */
   /*HashTable_insert(ClipMachineMemory->functions, fp, fp->hash_of_VarEntry); */
   HashTable_store(ClipMachineMemory->functions, fp, fp->hash_of_FuncEntry);
   /*if (HashTable_insert(ClipMachineMemory->functions, fp, fp->hash_of_VarEntry)) */
   /*if (HashTable_store(ClipMachineMemory->functions, fp, fp->hash_of_VarEntry))
      return 1;
      else
      {
      free(fp);
      return 0;
      }
    */
   return 1;
}
