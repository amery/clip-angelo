CLIP_DLLEXPORT ClipVar *
_clip_ref_public(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->publics, hash);

   if (!vp)
   {
      char buf[64];

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "try access to unknown public var '%s'", buf);
      return 0;
   }
   return &vp->ClipVar_var_of_VarEntry;
}
