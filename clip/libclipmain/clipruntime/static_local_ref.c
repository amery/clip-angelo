static ClipVar *
local_ref(ClipMachine * ClipMachineMemory, int no)
{
   ClipVar *vp;

   ClipVarFrame *lp = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;

   if (lp && no < lp->size_of_ClipVarFrame)
      vp = lp->ClipVar_vars_of_ClipVarFrame + no;
   else
   {
      int nlocals = no + 1;

      ClipVarFrame *localvars = (ClipVarFrame *) realloc(lp,
							 sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

      localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
      if (lp)
      {
	 memset(localvars->ClipVar_vars_of_ClipVarFrame + no, 0, (nlocals - localvars->size_of_ClipVarFrame) * sizeof(ClipVar));
      }
      else
      {
	 memset(localvars->ClipVar_vars_of_ClipVarFrame, 0, nlocals * sizeof(ClipVar));
	 localvars->refcount_of_ClipVarFrame = 1;
      }
      localvars->size_of_ClipVarFrame = nlocals;
      ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = localvars;

      vp = localvars->ClipVar_vars_of_ClipVarFrame + no;
   }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags
       && vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
   {
#if 1
      if (vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType ==
	  F_MPTR_ClipFlags
	  && (vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
	      ARRAY_type_of_ClipVarType || vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
      {
      }
      else
#endif
	 vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
   }

   return vp;
}
