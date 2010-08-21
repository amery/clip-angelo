int
clip_EMPTY(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   int r = 1, l;

   if (vp)
   {
      switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 if ((l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf))
	 {
	    char *s;

	    int i;

	    for (i = 0, s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf; i < l; ++i, ++s)
	    {
	       switch (*s)
	       {
	       case ' ':
	       case '\t':
	       case '\r':
	       case '\n':
		  continue;
	       }
	       r = 0;
	       break;
	    }
	 }
	 break;
      case NUMERIC_type_of_ClipVarType:
	 {
	    double d = _clip_double(vp);

	    r = (d == 0 ? 1 : 0);
	 }
	 break;
      case DATE_type_of_ClipVarType:
	 r = (vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0 ? 1 : 0);
	 break;
      case LOGICAL_type_of_ClipVarType:
	 r = (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 0 : 1);
	 break;
      case ARRAY_type_of_ClipVarType:
	 r = (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 0 ? 1 : 0);
	 break;
      case MAP_type_of_ClipVarType:
	 r = (vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar == 0 ? 1 : 0);
	 break;
      default:
	 r = 0;
	 break;
      case UNDEF_type_of_ClipVarType:
	 r = 1;
	 break;
      }
   }

   _clip_retl(ClipMachineMemory, r);
   return 0;
}
