CLIP_DLLEXPORT void
_clip_memvar_param(ClipMachine * ClipMachineMemory, long hash, int no)
{
   ClipVar *cp = 0;

   VarEntry *vp;

   if (no >= 0 && no < ClipMachineMemory->argc)
      cp = ClipMachineMemory->bp - (ClipMachineMemory->argc - no);

   vp = add_private(ClipMachineMemory, hash);
   if (cp)
   {
      _clip_destroy(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry);
      vp->ClipVar_var_of_VarEntry = *cp;
      if ((vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.field_of_ClipType)
      {
	 vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 vp->ClipVar_var_of_VarEntry.ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
      }
      CLEAR_CLIPVAR(cp);
   }
}
