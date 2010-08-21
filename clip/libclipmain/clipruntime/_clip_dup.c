CLIP_DLLEXPORT int
_clip_dup(ClipMachine * ClipMachineMemory, ClipVar * dest, ClipVar * src)
{
   dest->ClipType_t_of_ClipVar = src->ClipType_t_of_ClipVar;

   switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
   {
   case F_MPTR_ClipFlags:
   case F_MREF_ClipFlags:
#if 0
      {
	 enum ClipVarType type = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;

	 if (type == CCODE_type_of_ClipVarType || type == PCODE_type_of_ClipVarType)
	 {
	    dest->ClipRefVar_p_of_ClipVar.vp = src->ClipRefVar_p_of_ClipVar.vp;
	    ++(src->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipVect);
	    return 0;
	 }
      }
#endif
      src = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
      dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      break;
   case F_MSTAT_ClipFlags:
      /*
       *dest = *src;
       return 0;
       */
   case F_NONE_ClipFlags:
      break;
   }

   switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
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
      {
		/**dest = *src;*/
	 CLEAR_CLIPVAR(dest);
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	 dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = src;

	 src->ClipType_t_of_ClipVar.count_of_ClipType++;
#if 0
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->c = src->c;
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType && vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.block->file)
	    vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.block->file->refCount++;
#if 0
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
	 {
	    ClipBlock *bp;

	    bp = NEW(ClipBlock);
	    *bp = *(src->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.block);
	    vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.block = bp;
	    /*bp->file->refCount++; */
	 }
	 else
	    vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.func = src->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.func;
#endif
#endif
      }
#if 0
      if (src->ClipCodeVar_c_of_ClipVar.uplocals && src->ClipCodeVar_c_of_ClipVar.uplocals->refcount_of_ClipVarFrame)
	 src->ClipCodeVar_c_of_ClipVar.uplocals->refcount_of_ClipVarFrame++;
#endif
      break;
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
   case ARRAY_type_of_ClipVarType:
      {
	 ClipVar *ap = NEW(ClipVar);

	 int count = src->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar, i;

	 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
	 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	 ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * count);
	 ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = count;
	 for (i = 0; i < count; i++)
	 {
	    ClipVar *src_p, *tg_p;

	    tg_p = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
	    src_p = src->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
	    _clip_dup(ClipMachineMemory, tg_p, src_p);
	 }
      }
      break;
   case MAP_type_of_ClipVarType:
      {
	 ClipVar *ap = NEW(ClipVar);

	 int count = src->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, i;

	 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
	 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	 ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) malloc(sizeof(ClipVarEl) * count);
	 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = count;
	 for (i = 0; i < count; i++)
	 {
	    ClipVarEl *src_p, *tg_p;

	    tg_p = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;
	    src_p = src->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;
	    _clip_dup(ClipMachineMemory, &tg_p->ClipVar_var_of_ClipVarEl, &src_p->ClipVar_var_of_ClipVarEl);
	    tg_p->no_of_ClipVarEl = src_p->no_of_ClipVarEl;
	 }
      }
      break;
   default:
      break;
   }
   return 0;
}
