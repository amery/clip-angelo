static int
rm_cmp(ClipMachine * ClipMachineMemory, int oper, ClipVar * vp1, ClipVar * vp2, int ic)
{
   int r;

   if (ic && (vp1->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType) && (vp2->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType))
   {
      if (oper == RM_EEQU)
      {
	 if (vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf != vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf)
	    r = vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 else
	    r = _clip_strnncasecmp(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				   vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				   vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      }
      else
      {
	 if (!vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf && vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf)
	    r = -1;
	 else
	    r = _clip_strncasecmp(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				  vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				  min(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf));
      }
   }
   else
   {
      _clip_cmp(ClipMachineMemory, vp1, vp2, &r, 1);
   }
   switch (oper)
   {
   case RM_EQU:
      return r;
   case RM_EEQU:
      return r;
   case RM_NEQU:
      return !r;
   case RM_LAR:
      return !(r > 0);
   case RM_LARE:
      return !(r >= 0);
   case RM_LES:
      return !(r < 0);
   case RM_LESE:
      return !(r <= 0);
   }
   return 0;
}
