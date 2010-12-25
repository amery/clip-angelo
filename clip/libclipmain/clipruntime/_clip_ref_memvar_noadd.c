CLIP_DLLEXPORT ClipVar *
_clip_ref_memvar_noadd(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp = fetch_var(ClipMachineMemory, hash);

   if (!vp)
   {
#if 0
      return 0;
#else
      char buf[64];

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no memvar variable '%s'", buf);
      _clip_call_errblock(ClipMachineMemory, EG_NOVAR);
      return 0;
#endif
   }
   return &vp->ClipVar_var_of_VarEntry;
}
