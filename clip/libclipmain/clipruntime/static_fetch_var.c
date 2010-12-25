static VarEntry *
fetch_var(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);

   if (!vp)
      vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->publics, hash);
   return vp;
}
