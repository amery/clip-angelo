static long
calc_casehash(ClipMachine * ClipMachineMemory, ClipVar * vp, int trim)
{
   if (!vp)
      return 0;
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      {
	 char *s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 int l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 if (trim)
	 {
	    char *e;

	    for (e = s + l - 1; e >= s; e--)
	    {
	       switch (*e)
	       {
	       case ' ':
	       case '\t':
	       case '\r':
	       case '\n':
		  l--;
		  continue;
	       }
	       break;
	    }
	 }
	 return _clip_casehashbytes(0, s, l);
      }
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
