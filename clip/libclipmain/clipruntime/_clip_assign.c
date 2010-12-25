CLIP_DLLEXPORT int
_clip_assign(struct ClipMachine *ClipMachineMemory, void *Lval)
{
   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *lval = (ClipVar *) Lval;

   int ret;

   if (lval)
   {
      if ((lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && lval->ClipType_t_of_ClipVar.field_of_ClipType && lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
      {
	 return _clip_assign_field(ClipMachineMemory,
				   lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef,
				   lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef);
      }
      else
      {
	 if (!(lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	     && (rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
	     && lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType
	     && !(rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
	 {
	    _clip_dup(ClipMachineMemory, lval, _clip_vptr(rval));
	 }
#if 0
	 else if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
	 {
	    _clip_destroy(ClipMachineMemory, lval);
	    ret = _clip_mclone(ClipMachineMemory, lval, rval);
	 }
#endif
	 else if ((ret = _clip_mclone(ClipMachineMemory, lval, rval)))
	    return ret;
      }
      lval->ClipType_t_of_ClipVar.field_of_ClipType = 0;
      if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	 lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   }

   _clip_destroy(ClipMachineMemory, rval);
   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame--;
   return 0;
}
