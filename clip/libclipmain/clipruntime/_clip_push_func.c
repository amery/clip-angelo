CLIP_DLLEXPORT void
_clip_push_func(struct ClipMachine *ClipMachineMemory, ClipFunction f, int nlocals, int nolocals)
{
   ClipVar *sp;

   ClipVar *vp;

   ClipFrame *fp = ClipMachineMemory->fp;

   vp = NEW(ClipVar);

   vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = f;

   if (nlocals)
   {
      ClipVarFrame *localvars = (ClipVarFrame *) calloc(1,
							sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

      localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
      memcpy(localvars->ClipVar_vars_of_ClipVarFrame, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - nlocals, nlocals * sizeof(ClipVar));
      localvars->refcount_of_ClipVarFrame = 1;
      localvars->size_of_ClipVarFrame = nlocals;
      vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = localvars;
      ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= nlocals;
   }
   else if (!nolocals && fp->ClipVarFrame_localvars_of_ClipFrame && fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame > 0)
   {
      fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame++;
      vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = fp->ClipVarFrame_localvars_of_ClipFrame;
   }

   sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;

   CLEAR_CLIPVAR(sp);
   sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
   sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

}
