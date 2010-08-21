static int
cdx_ii_key(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   memset(v, 0, sizeof(ClipVar));
   if (ro->eof)
   {
      switch (ro->type)
      {
      case 'C':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(ro->bufsize);
	 if (ro->binary)
	    memset(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 0, ro->bufsize);
	 else
	    memset(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', ro->bufsize);
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
	 break;
      case 'N':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	 v->ClipType_t_of_ClipVar.len_of_ClipType = ro->keysize;
	 v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
	 break;
      case 'D':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	 break;
      case 'T':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	 v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = 0;
	 v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = 0;
	 break;
      case 'L':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	 break;
      }
   }
   else
   {
      switch (ro->type)
      {
      case 'C':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(ro->bufsize);
	 memcpy(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ro->iikey, ro->bufsize);
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
	 break;
      case 'N':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _cdx_double(*(unsigned long long *) ro->iikey);
	 v->ClipType_t_of_ClipVar.len_of_ClipType = ro->keysize;
	 v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
	 break;
      case 'D':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _cdx_double(*(unsigned long long *) ro->iikey);
	 break;
      case 'T':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	 v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) ro->iikey);
	 v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) ro->iikey + 4);
	 break;
      case 'L':
	 v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (ro->iikey[0] == type_weight(LOGICAL_type_of_ClipVarType)) ? 1 : 0;
	 break;
      }
   }
   return 0;
}
