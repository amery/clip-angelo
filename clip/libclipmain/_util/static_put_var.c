static void
put_var(ClipMachine * ClipMachineMemory, ClipVar * vp, OutBuf * bp, Coll * refs)
{
   vp = _clip_vptr(vp);

   if (search_Coll(refs, vp, 0))
   {
      putByte_Buf(bp, UNDEF_type_of_ClipVarType);
      return;
   }

   putByte_Buf(bp, vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType);
   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case UNDEF_type_of_ClipVarType:
      break;
   case NUMERIC_type_of_ClipVarType:
      {
	 putByte_Buf(bp, vp->ClipType_t_of_ClipVar.dec_of_ClipType);
	 putByte_Buf(bp, vp->ClipType_t_of_ClipVar.len_of_ClipType);
	 putByte_Buf(bp, vp->ClipType_t_of_ClipVar.memo_of_ClipType);
	 if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    char *buf;

	    if (vp->ClipType_t_of_ClipVar.dec_of_ClipType)
	       buf = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
	    else
	       buf = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMachineMemory->decimals, 0);
	    put_str(bp, buf, strlen(buf));
	    free(buf);
	 }
	 else
	 {
	    char buf[48];

	    if (vp->ClipType_t_of_ClipVar.len_of_ClipType < sizeof(buf))
	    {
	       char *s;

	       snprintf(buf, sizeof(buf), "%*.*f", vp->ClipType_t_of_ClipVar.len_of_ClipType, vp->ClipType_t_of_ClipVar.dec_of_ClipType, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	       for (s = buf + strlen(buf); s > buf; --s)
		  if ((*s) == ',')
		     (*s) = '.';
	    }
	    else
	       _clip_dtos(vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, buf, sizeof(buf), 0);
	    put_str(bp, buf, strlen(buf));
	 }
      }
      break;
   case CHARACTER_type_of_ClipVarType:
      put_str(bp, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      break;
   case LOGICAL_type_of_ClipVarType:
      putByte_Buf(bp, vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
      break;
   case DATE_type_of_ClipVarType:
      putInt32_Buf(bp, htonl(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar));
      break;
   case OBJECT_type_of_ClipVarType:
      vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->print_of_ClipObjRtti(ClipMachineMemory,
									   vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar,
									   vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, &ClipMachineMemory->buf, &ClipMachineMemory->buflen);
      put_str(bp, ClipMachineMemory->buf, ClipMachineMemory->buflen);
      break;
   case PCODE_type_of_ClipVarType:
   case CCODE_type_of_ClipVarType:
      /*out_any(ClipMachineMemory, "CODE", 4, attr, dev); */
      break;
   case ARRAY_type_of_ClipVarType:
      {
	 int i;

	 insert_Coll(refs, vp);
	 putInt32_Buf(bp, htonl(vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar));
	 for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; ++i)
	    put_var(ClipMachineMemory, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, bp, refs);
      }
      break;
   case MAP_type_of_ClipVarType:
      {
	 int i;

	 insert_Coll(refs, vp);
	 putInt32_Buf(bp, htonl(vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar));
	 for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; ++i)
	 {
	    putInt32_Buf(bp, htonl(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl));
	    put_var(ClipMachineMemory, &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl), bp, refs);
	 }
      }
      break;
   default:
      ;
   }
}
