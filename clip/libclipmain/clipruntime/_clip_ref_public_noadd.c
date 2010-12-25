CLIP_DLLEXPORT ClipVar *
_clip_ref_public_noadd(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->publics, hash);

   if (!vp)
      return 0;
   else
      return &vp->ClipVar_var_of_VarEntry;
}
