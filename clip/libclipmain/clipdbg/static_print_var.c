static void
print_var(ClipMachine * ClipMachineMemory, ClipVar * vp, long *dim, int ndim, int binary)
{
   if (!ndim)
      print_dbg(ClipMachineMemory, vp, binary);
   else
   {
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 ClipVar var;

	 memset(&var, 0, sizeof(var));
	 if (_clip_aget(ClipMachineMemory, vp, &var, ndim, dim))
	 {
	    if (!binary)
	       fprintf(_clip_dbg_out, "Bad index.\n");
	 }
	 else
	    print_dbg(ClipMachineMemory, &var, binary);
	 _clip_destroy(ClipMachineMemory, &var);
      }
      else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      {
	 if (ndim > 1)
	 {
	    if (!binary)
	       fprintf(_clip_dbg_out, "Too deep index.\n");
	 }
	 else
	 {
	    ClipVar *vpp = _clip_mget(ClipMachineMemory, vp, dim[0]);

	    if (vpp)
	       print_dbg(ClipMachineMemory, vpp, binary);
	    else if (!binary)
	       fprintf(_clip_dbg_out, "Bad index.\n");
	 }
      }
      else if (!binary)
      {
	 fprintf(_clip_dbg_out, "Not ARRAY or MAP\n");
      }
   }
}
