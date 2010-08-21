CLIP_DLLEXPORT int
_clip_memvarf(ClipMachine * ClipMachineMemory, long hash)
{
   VarEntry *vp;

   vp = fetch_var(ClipMachineMemory, hash);

   if (!vp)
   {
      char buf[64];

      /*int r; */

      if (!_clip_try_field(ClipMachineMemory, hash))
	 return 0;

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no variable '%s'", buf);
      /*r = _clip_trap_err(ClipMachineMemory, EG_NOVAR, 0, 0, 0, 0, buf); */
      return _clip_call_errblock(ClipMachineMemory, EG_NOVAR);
   }
   _clip_push(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry);
   return 0;
}
