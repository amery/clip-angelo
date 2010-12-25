CLIP_DLLEXPORT void
_clip_memvar_public(ClipMachine * ClipMachineMemory, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMachineMemory->publics, hash));
   if (!vp)
   {
      VarEntry *ep = new_VarEntry(hash);

      ep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      HashTable_insert(ClipMachineMemory->publics, ep, hash);
   }
}
