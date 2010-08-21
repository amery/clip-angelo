void
_clip_print_dbg(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   const char *nm = _clip_type_name(vp);

   if (_clip_dbg_out)
      fprintf(_clip_dbg_out, "%s; ", nm);
   print_var(ClipMachineMemory, vp, 0, DEV_DBG, 0);
}
