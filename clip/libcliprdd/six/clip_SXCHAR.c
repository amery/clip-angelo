int
clip_SXCHAR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SXCHAR";

   int len = _clip_parni(ClipMachineMemory, 1);

   ClipVar *v = _clip_par(ClipMachineMemory, 2);

   ClipVar *r = RETPTR(ClipMachineMemory);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (len < 0)
      len = 0;
   memset(r, 0, sizeof(ClipVar));
   r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(len + 1);
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   switch (_clip_parinfo(ClipMachineMemory, 2))
   {
   case CHARACTER_type_of_ClipVarType:
      {
	 int l = min(len, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	 memcpy(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	 memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + l, ' ', len - l);
	 break;
      }
   case NUMERIC_type_of_ClipVarType:
      {
	 int d = min(v->ClipType_t_of_ClipVar.dec_of_ClipType,
		     len - (v->ClipType_t_of_ClipVar.len_of_ClipType - v->ClipType_t_of_ClipVar.dec_of_ClipType));

	 char *s = r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 _clip_dtostr(s, len, d, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0);
	 while (*s == ' ')
	    s++;
	 memmove(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, s, len - (s - r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
	 memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len -
		(s - r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), ' ', (s - r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
	 break;
      }
   case DATE_type_of_ClipVarType:
      {
	 char *s = _clip_date_to_str(v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, "yyyymmdd");

	 int l = min(len, strlen(s));

	 memcpy(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, s, l);
	 memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + l, ' ', len - l);
	 free(s);
	 break;
      }
   case LOGICAL_type_of_ClipVarType:
      {
	 if (len > 0)
	 {
	    *r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
	    memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + 1, ' ', len - 1);
	 }
	 break;
      }
   default:
      memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', len);
      break;
   }
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[len] = 0;
   return 0;
 err:
   return er;
}
