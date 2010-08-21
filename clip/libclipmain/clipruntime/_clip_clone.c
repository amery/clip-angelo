CLIP_DLLEXPORT int
_clip_clone(struct ClipMachine *ClipMachineMemory, ClipVar * dest, ClipVar * src)
{
   ClipType t;

   CLEAR_CLIPVAR(dest);
   if (!src)
      return 0;

   t = src->ClipType_t_of_ClipVar;
   switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
   {
   case F_MPTR_ClipFlags:
      dup_ref(dest, src);
      return 0;
   case F_MREF_ClipFlags:
      switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case MAP_type_of_ClipVarType:
      case ARRAY_type_of_ClipVarType:
      case CCODE_type_of_ClipVarType:
      case PCODE_type_of_ClipVarType:
	 dup_ref(dest, src);
	 return 0;
      default:
	 return _clip_clone(ClipMachineMemory, dest, src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
      }
   case F_MSTAT_ClipFlags:
      t.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   case F_NONE_ClipFlags:
      switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case UNDEF_type_of_ClipVarType:
	 break;
      case NUMERIC_type_of_ClipVarType:
	 if (src->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    dest->ClipType_t_of_ClipVar.len_of_ClipType = src->ClipType_t_of_ClipVar.len_of_ClipType;
	    dest->ClipType_t_of_ClipVar.dec_of_ClipType = src->ClipType_t_of_ClipVar.dec_of_ClipType;
	    dest->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	    dest->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = rational_copy(src->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 }
	 else
	    *dest = *src;
	 break;
      case LOGICAL_type_of_ClipVarType:
      case DATE_type_of_ClipVarType:
      case DATETIME_type_of_ClipVarType:
	 *dest = *src;
	 break;
      case CCODE_type_of_ClipVarType:
      case PCODE_type_of_ClipVarType:
	 *dest = *src;
	 if (src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar && src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount_of_ClipVarFrame)
	    src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount_of_ClipVarFrame++;
	 return 0;
      case CHARACTER_type_of_ClipVarType:
	 {
	    int len = src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	    dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) _clip_memdup(src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	    dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;

	 }
	 break;
      case OBJECT_type_of_ClipVarType:
	 src->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->clone_of_ClipObjRtti(ClipMachineMemory, dest, src);
	 break;
      default:
	 break;
      }
   }
   dest->ClipType_t_of_ClipVar = t;
   return 0;
}
