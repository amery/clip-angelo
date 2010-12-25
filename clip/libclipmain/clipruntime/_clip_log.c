
CLIP_DLLEXPORT int
_clip_log(ClipVar * vp)
{
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case NUMERIC_type_of_ClipVarType:
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 return rational_empty(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 return vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0 ? 0 : 1;
   case LOGICAL_type_of_ClipVarType:
      return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   case CHARACTER_type_of_ClipVarType:
      return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf == 0 ? 0 : 1;
   case ARRAY_type_of_ClipVarType:
      return vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 0 ? 0 : 1;
   case MAP_type_of_ClipVarType:
      return vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar == 0 ? 0 : 1;
   default:
      break;
   }
   return 0;
}
