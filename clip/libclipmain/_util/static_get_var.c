static int
get_var(ClipMachine * ClipMachineMemory, ClipVar * vp, char **str, long *len)
{
   int type;

   memset(vp, 0, sizeof(ClipVar));
   if (!get_byte(str, len, &type))
      return -1;

   switch (type)
   {
   case UNDEF_type_of_ClipVarType:
      break;
   case NUMERIC_type_of_ClipVarType:
      {
	 int dec, l, memo;

	 char *s = 0;

	 long sl = 0;

	 if (!get_byte(str, len, &dec))
	    return -1;
	 if (!get_byte(str, len, &l))
	    return -1;
	 if (!get_byte(str, len, &memo))
	    return -1;
	 if (!get_str(str, len, &s, &sl))
	    return -1;

	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.len_of_ClipType = l;
	 vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = memo;
	 if (memo)
	    vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = rational_fromString(s);
	 else
	 {
	    int dec;

	    vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _clip_strtod(s, &dec);;
	 }
	 free(s);
      }
      break;
   case CHARACTER_type_of_ClipVarType:
      {
	 char *s = 0;

	 long sl = 0;

	 if (!get_str(str, len, &s, &sl))
	    return -1;
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

	 /*
	    vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf = malloc(sl);
	    memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf,s,sl);
	    free(s);
	  */
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = sl;
      }
      break;
   case LOGICAL_type_of_ClipVarType:
      {
	 int n;

	 if (!get_byte(str, len, &n))
	    return -1;
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = n;
      }
      break;
   case DATE_type_of_ClipVarType:
      {
	 long n;

	 if (!get_int32(str, len, &n))
	    return -1;
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = n;
      }
      break;
   case OBJECT_type_of_ClipVarType:
      {
	 char *s = 0;

	 long sl = 0;

	 if (!get_str(str, len, &s, &sl))
	    return -1;
	 /*vp->o.rtti->print(ClipMachineMemory, vp->o.obj, vp->o.rtti, &ClipMachineMemory->buf, &ClipMachineMemory->buflen); */
	 free(s);
      }
      break;
   case PCODE_type_of_ClipVarType:
   case CCODE_type_of_ClipVarType:
      {
	 /*ClipVar *sp = (ClipVar *) calloc(1, sizeof(ClipVar)); */
	 /*
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR;
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_t;
	    vp->ClipRefVar_p_of_ClipVar.vp = sp;
	  */

	 /*sp->ClipType_t_of_ClipVar.count_of_ClipType = 1; */
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = null_func;
      }
      break;
   case ARRAY_type_of_ClipVarType:
      {
	 long i, size;

	 ClipVar *ap;

	 if (!get_int32(str, len, &size))
	    return -1;

	 ap = (ClipVar *) calloc(1, sizeof(ClipVar));
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	 ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * size);
	 ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = size;

	 for (i = 0; i < size; ++i)
	    if (get_var(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, str, len) < 0)
	       return -1;
      }
      break;
   case MAP_type_of_ClipVarType:
      {
	 int i;

	 long size;

	 ClipVar *ap;

	 if (!get_int32(str, len, &size))
	    return -1;

	 ap = (ClipVar *) calloc(1, sizeof(ClipVar));
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	 ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) calloc(sizeof(ClipVarEl), size);
	 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = size;

	 for (i = 0; i < size; ++i)
	 {
	    if (!get_int32(str, len, &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl)))
	       return -1;
	    if (get_var(ClipMachineMemory, &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl), str, len) < 0)
	       return -1;
	 }
      }
      break;
   default:
      ;
   }
   return 0;
}
