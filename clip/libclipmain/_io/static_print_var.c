static void
print_var(ClipMachine * ClipMachineMemory, ClipVar * vp, int attr, int dev, int level)
{
   if (level >= ClipMachineMemory->print_max_deep)
   {
      char b[64];

      int l;

      sprintf(b, "Max print level reached: %d ", level);
      l = strlen(b);
      if (dev != DEV_LOG)
	 out_dev(ClipMachineMemory, b, l, attr, dev);
      _clip_out_log(b, l);
      return;
   }

   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case UNDEF_type_of_ClipVarType:
      out_any(ClipMachineMemory, "NIL", 3, attr, dev);
      break;
   case NUMERIC_type_of_ClipVarType:
      {
	 if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    char *buf;

	    if (vp->ClipType_t_of_ClipVar.dec_of_ClipType)
	       buf = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
	    else
	       buf = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMachineMemory->decimals, 0);
	    out_any(ClipMachineMemory, buf, strlen(buf), attr, dev);
	    free(buf);
	 }
	 else
	 {
	    char buf[48];

	    char *s;

	    if (vp->ClipType_t_of_ClipVar.len_of_ClipType < sizeof(buf))
	       snprintf(buf, sizeof(buf), "%*.*f", vp->ClipType_t_of_ClipVar.len_of_ClipType, vp->ClipType_t_of_ClipVar.dec_of_ClipType, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    else
	       _clip_dtos(vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, buf, sizeof(buf), 0);
	    for (s = buf + strlen(buf); s > buf; --s)
	       if ((*s) == ',')
		  (*s) = '.';
	    out_any(ClipMachineMemory, buf, strlen(buf), attr, dev);
	 }
      }
      break;
   case CHARACTER_type_of_ClipVarType:
      out_any(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, attr, dev);
      break;
   case LOGICAL_type_of_ClipVarType:
      if (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 out_any(ClipMachineMemory, ".T.", 3, attr, dev);
      else
	 out_any(ClipMachineMemory, ".F.", 3, attr, dev);
      break;
   case DATE_type_of_ClipVarType:
      {
	 char *s = _clip_date_to_str(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, ClipMachineMemory->date_format);

	 out_any(ClipMachineMemory, s, strlen(s), attr, dev);
	 free(s);
      }
      break;
   case DATETIME_type_of_ClipVarType:
      {
	 int len;

	 char *s = _clip_ttoc(ClipMachineMemory, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar,
			      vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &len,
			      ClipMachineMemory->date_format, ClipMachineMemory->hours,
			      ClipMachineMemory->seconds);

	 out_any(ClipMachineMemory, s, len, attr, dev);
	 free(s);
      }
      break;
   case OBJECT_type_of_ClipVarType:
      vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->print_of_ClipObjRtti(ClipMachineMemory,
									   vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar,
									   vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, &ClipMachineMemory->buf, &ClipMachineMemory->buflen);
      out_any(ClipMachineMemory, ClipMachineMemory->buf, ClipMachineMemory->buflen, attr, dev);
      break;
   case PCODE_type_of_ClipVarType:
   case CCODE_type_of_ClipVarType:
      out_any(ClipMachineMemory, "CODE", 4, attr, dev);
      break;
   case ARRAY_type_of_ClipVarType:
      {
	 int i;

	 out_any(ClipMachineMemory, "{", 1, attr, dev);
	 for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; ++i)
	 {
	    if (i)
	       out_any(ClipMachineMemory, ", ", 2, attr, dev);
	    print_var(ClipMachineMemory, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, attr, dev, level + 1);
	 }
	 out_any(ClipMachineMemory, "}", 1, attr, dev);
      }
      break;
   case MAP_type_of_ClipVarType:
      {
	 int i;

	 out_any(ClipMachineMemory, "{", 1, attr, dev);
	 for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; ++i)
	 {
	    char buf[64];

	    int l;

	    if (i)
	       out_any(ClipMachineMemory, ", ", 2, attr, dev);
	    _clip_hash_name(ClipMachineMemory, vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl, buf, sizeof(buf));
	    l = strlen(buf);
	    snprintf(buf + l, sizeof(buf) - l, ":");
	    l += 1;
	    out_any(ClipMachineMemory, buf, l, attr, dev);
	    print_var(ClipMachineMemory, &vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl, attr, dev, level + 1);
	 }
	 out_any(ClipMachineMemory, "}", 1, attr, dev);
      }
      break;
   default:
      break;
   }
}
