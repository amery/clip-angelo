static int
ntx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__)
{
   int len, dd, mm, yy, ww;

   switch (var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
      memcpy(res, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
      memset(res + len, 32, ro->keysize - len);
      break;
   case NUMERIC_type_of_ClipVarType:
      if (var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 0)
      {
	 _clip_dtostr(res, ro->keysize, ro->dec, var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
      }
      else
      {
	 char *s;

	 _clip_dtostr(res, ro->keysize, ro->dec, -var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
	 for (s = res; *s; ++s)
	    switch (*s)
	    {
	    case '0':
	       *s = ',';
	       break;
	    case '1':
	       *s = '+';
	       break;
	    case '2':
	       *s = '*';
	       break;
	    case '3':
	       *s = ')';
	       break;
	    case '4':
	       *s = '(';
	       break;
	    case '5':
	       *s = '\'';
	       break;
	    case '6':
	       *s = '&';
	       break;
	    case '7':
	       *s = '%';
	       break;
	    case '8':
	       *s = '$';
	       break;
	    case '9':
	       *s = '#';
	       break;
	    }
      }
      break;
   case DATE_type_of_ClipVarType:
      if (var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0)
      {
	 strcpy(res, "00000000");
      }
      else
      {
	 _clip_cdate(var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	 sprintf(res, "%04d%02d%02d", yy, mm, dd);
      }
      break;
   case LOGICAL_type_of_ClipVarType:
      ((char *) res)[0] = var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
      break;
   default:
      ((char *) res)[0] = 0;
      break;
   }
   return 0;
}
