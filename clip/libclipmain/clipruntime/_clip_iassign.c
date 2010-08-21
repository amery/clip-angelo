CLIP_DLLEXPORT int
_clip_iassign(struct ClipMachine *ClipMachineMemory, void *Lval)
{
   int ret;

   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *lval = (ClipVar *) Lval;

   if ((lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && lval->ClipType_t_of_ClipVar.field_of_ClipType && lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
   {
      ret =
	 _clip_iassign_field(ClipMachineMemory,
			     lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef, lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef);
   }
   else
   {
      if (!(lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	  && (rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
	  && lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType
	  && !(rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
      {
	 ret = _clip_dup(ClipMachineMemory, lval, _clip_vptr(rval));
      }
      else
	 ret = _clip_mclone(ClipMachineMemory, lval, rval);
      lval->ClipType_t_of_ClipVar.field_of_ClipType = 0;
      if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	 lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   }

   return ret;
}
