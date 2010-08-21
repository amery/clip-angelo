CLIP_DLLEXPORT int
_clip_memvar(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp;

   vp = fetch_var(ClipMachineMemory, hash);

   if (!vp)
   {
#if 0
      vp = add_private(ClipMachineMemory, hash);
#else
      char buf[64];

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no memvar variable '%s'", buf);
      return _clip_call_errblock(ClipMachineMemory, EG_NOVAR);
#endif
   }
   _clip_push(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry);
   return 0;
}
