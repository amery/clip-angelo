CLIP_DLLEXPORT int
_clip_register_hash(ClipMachine * ClipMachineMemory, ClipFunction func, long hash)
{
   NEWVAR(FuncEntry, fp);
   fp->ClipFunction_f_of_FuncEntry = func;
   fp->hash_of_FuncEntry = hash;
#if 0
   if (HashTable_insert(ClipMachineMemory->functions, fp, fp->hash_of_VarEntry))
      return 1;
   else
   {
      free(fp);
      return 0;
   }
#else
   HashTable_store(ClipMachineMemory->functions, fp, fp->hash_of_FuncEntry);
   return 1;
#endif
}
