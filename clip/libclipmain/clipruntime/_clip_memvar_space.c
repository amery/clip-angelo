CLIP_DLLEXPORT ClipVar *
_clip_memvar_space(ClipMachine * ClipMachineMemory, struct _HashTable *hp, long hash, int newvar)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(hp, hash);

   if (!vp)
   {
      if (!newvar)
	 return 0;
      vp = new_VarEntry(hash);
      HashTable_insert(hp, vp, hash);
   }

   return &(vp->ClipVar_var_of_VarEntry);
}
