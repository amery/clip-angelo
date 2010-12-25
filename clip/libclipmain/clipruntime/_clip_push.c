CLIP_DLLEXPORT int
_clip_push(ClipMachine * ClipMachineMemory, void *vpp)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   ClipVar *vp = (ClipVar *) vpp;

   int ret;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType
       && (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && vp->ClipType_t_of_ClipVar.field_of_ClipType && vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
   {
      ret =
	 _clip_take_field(ClipMachineMemory, vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef,
			  vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef, vp);
      if (ret)
	 return ret;
   }

   ret = _clip_clone(ClipMachineMemory, sp, vp);

   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);

   /*
      if (!(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG))
      {
    */
   if (!ret)
   {
      if (sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && sp->ClipType_t_of_ClipVar.len_of_ClipType)
	 ret = _clip_expand_var(ClipMachineMemory, sp);
   }
   /*} */

   CLIP_CHECK_STACK;

   return ret;
}
