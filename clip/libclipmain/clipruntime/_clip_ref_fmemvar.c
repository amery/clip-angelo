CLIP_DLLEXPORT int
_clip_ref_fmemvar(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp;

   if (!_clip_try_field(ClipMachineMemory, hash))
      return 0;

   vp = fetch_var(ClipMachineMemory, hash);

   if (!vp)
   {
#if 1
      vp = add_private(ClipMachineMemory, hash);
#else
      char buf[64];

      /*int r; */

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no variable '%s'", buf);
      /*r = _clip_trap_err(ClipMachineMemory, EG_NOVAR, 0, 0, 0, 0, buf); */
      return _clip_call_errblock(ClipMachineMemory, EG_NOVAR);
#endif
   }
   _clip_ref(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry, 0);
   return 0;
}
