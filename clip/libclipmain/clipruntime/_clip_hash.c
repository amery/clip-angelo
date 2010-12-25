CLIP_DLLEXPORT long
_clip_hash(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      return _clip_hashbytes(0, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
   case NUMERIC_type_of_ClipVarType:
      return _clip_double(vp);
   case DATE_type_of_ClipVarType:
      return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
   case LOGICAL_type_of_ClipVarType:
      return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   case OBJECT_type_of_ClipVarType:
      return vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->hash_of_ClipObjRtti(ClipMachineMemory, vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar);
   default:
      break;
   }
   return 0;
}
