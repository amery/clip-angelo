static int
_ntx_calc_key(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int er;

   if (ro->simpexpr)
   {
      ClipVar v;

      memset(&v, 0, sizeof(ClipVar));
      if ((er = rdd_takevalue(ClipMachineMemory, rd, ro->simpfno, &v, __PROC__)))
	 return er;
      if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 memcpy(ro->key, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      }
      else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 if (v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 0)
	 {
	    _clip_dtostr(ro->key, ro->keysize, ro->dec, v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
	 }
	 else
	 {
	    char *s;

	    _clip_dtostr(ro->key, ro->keysize, ro->dec, -v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
	    for (s = ro->key; *s; ++s)
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
      }
      else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
      {
	 int dd, mm, yy, ww;

	 if (v.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0)
	 {
	    strcpy(ro->key, "00000000");
	 }
	 else
	 {
	    _clip_cdate(v.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	    sprintf(ro->key, "%04d%02d%02d", yy, mm, dd);
	 }
      }
      else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
      {
	 ro->key[0] = v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
      }
      _clip_destroy(ClipMachineMemory, &v);
   }
   else
   {
      ClipVar vv, *vp;

      char ckey[1024];

      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	 return er;
      vp = _clip_vptr(&vv);
      if ((er = ntx_formatkey(ClipMachineMemory, ro, vp, ckey, __PROC__)))
	 return er;
      memcpy(ro->key, ckey, ro->keysize);
      _clip_destroy(ClipMachineMemory, &vv);
   }
   return 0;
}
