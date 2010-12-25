static void
_cdx_keyvar(RDD_ORDER * ro, void *key, char *lockey, ClipVar * v)
{
   switch (ro->type)
   {
   case 'C':
      v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
      if (ro->binary)
      {
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) key;
      }
      else
      {
	 memcpy(lockey, key, ro->bufsize);
	 loc_read(ro->index->loc, (unsigned char *) lockey, ro->bufsize);
	 v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = lockey;
      }
      break;
   case 'N':
      v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _cdx_double(*(unsigned long long *) key);
      v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
      break;
   case 'D':
      v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
      v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = (long) _cdx_double(*(unsigned long long *) key);
      break;
   case 'T':
      v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
      v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) key);
      v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) key + 4);
      break;
   case 'L':
      v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = ((*(char *) key) == 'T');
      break;
   default:
      memset(v, 0, sizeof(ClipVar));
      break;
   }
}
