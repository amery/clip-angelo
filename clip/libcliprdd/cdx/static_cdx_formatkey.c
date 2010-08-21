static int
cdx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__)
{
   memset(res, 0, ro->bufsize);
   switch (var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      {
	 int len;

	 if (ro->type == 'X')
	 {
	    len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - 1);
	    *(char *) res = type_weight(CHARACTER_type_of_ClipVarType);
	    memcpy(res + 1, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	    if (ro->ic && !ro->binary)
	       _clip_upstr(res + 1, len);
	    memset(res + len + 1, 0, ro->bufsize - 1 - len);
	 }
	 else
	 {
	    len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize);
	    memcpy(res, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	    if (ro->ic)
	       _clip_upstr(res, len);
	    memset(res + len, ' ', ro->bufsize - len);
	 }
	 break;
      }
   case NUMERIC_type_of_ClipVarType:
      {
	 if (ro->type == 'X')
	 {
	    char *cres = (char *) res;

	    *(char *) res = type_weight(NUMERIC_type_of_ClipVarType);
	    *(((char *) res) + 1) = 0;
	    cres++;
	    res = (void *) cres;
	 }
	 if (var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0)
	    var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	 *(unsigned long long *) res = _cdx_longlong(var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 break;
      }
   case DATE_type_of_ClipVarType:
      {
	 if (ro->type == 'X')
	 {
	    char *cres = (char *) res;

	    *(char *) res = type_weight(DATE_type_of_ClipVarType);
	    cres++;
	    res = (void *) cres;
	 }
	 *(unsigned long long *) res = _cdx_longlong((double) var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar);
	 break;
      }
   case DATETIME_type_of_ClipVarType:
      {
	 if (ro->type == 'X')
	 {
	    char *cres = (char *) res;

	    *(char *) res = type_weight(DATETIME_type_of_ClipVarType);
	    cres++;
	    res = (void *) cres;
	 }
	 _rdd_put_backuint((unsigned char *) res, var->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
	 _rdd_put_backuint((unsigned char *) res + 4, var->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
	 break;
      }
   case LOGICAL_type_of_ClipVarType:
      {
	 if (ro->type == 'X')
	 {
	    char *cres = (char *) res;

	    *(char *) res = type_weight(LOGICAL_type_of_ClipVarType);
	    cres++;
	    res = (void *) cres;
	 }
	 *(char *) res = var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
	 break;
      }
   default:
      break;
   }
   return 0;
}
