int
rdd_takevalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vv, const char *__PROC__)
{
   int r, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if (rd->eof)
   {
      switch (rd->fields[no].type)
      {
      case 'C':
	 vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len;
	 vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) malloc(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 memset(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 32, vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
	 break;
      case 'M':
	 vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vv->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	 vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;
	 vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) calloc(1, 1);
	 break;
      case 'N':
	 vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 vv->ClipType_t_of_ClipVar.len_of_ClipType = rd->fields[no].len;
	 vv->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	 vv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	 break;
      case 'D':
	 vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 vv->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	 break;
      case 'L':
	 vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 vv->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	 break;
      case 'V':
	 if (rd->fields[no].len == 3)
	 {
	    vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	    vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    vv->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	 }
	 else if (rd->fields[no].len == 4)
	 {
	    vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    vv->ClipType_t_of_ClipVar.len_of_ClipType = 10;
	    vv->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	    vv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	 }
	 else if (rd->fields[no].len >= 6)
	 {
	    vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	    vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len - 6;
	    vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) malloc(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	    memset(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 32, vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
	 }
	 break;
      }
   }
   else
   {
      if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, no, vv, __PROC__)))
	 return er;
   }
   if ((er = rdd_event(ClipMachineMemory, EVENT_GET, rd->area, no + 1, vv, &r, __PROC__)))
      return er;
   return 0;
}
