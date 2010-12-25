static int
dbf_getvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__)
{
   char *str;

   char l;

   char date[9];

   int yy = 0, mm = 0, dd = 0;

   char memo[11];

   int er;

   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if (rd->fields[no].nullable)
   {
      int nbyte = no >> 3;

      unsigned char byte;

      memcpy(&byte, (char *) rd->record + rd->nulloffs + nbyte, 1);
      if (byte & (1 << (no - (nbyte << 3))))
      {
	 memset(vp, 0, sizeof(ClipVar));
	 return 0;
      }
   }
   switch (rd->fields[no].type)
   {
   case 'C':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      str = (char *) malloc(rd->fields[no].len + 1);
      memcpy(str, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
      str[rd->fields[no].len] = 0;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = str;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len;
      if (!rd->fields[no].binary)
	 loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      break;
   case 'M':
   case 'P':
   case 'G':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
      if (rd->memo)
      {
	 if (rd->sig != 0x30)
	 {			/* Not VFP */
	    memcpy(memo, (char *) rd->record + rd->fields[no].offs, 10);
	    memo[10] = 0;
	    if ((er = rd->memo->vtbl->getvalue(ClipMachineMemory, rd->memo, atoi(memo), vp, __PROC__)))
	       return er;
	 }
	 else
	 {
	    memcpy(memo, (char *) rd->record + rd->fields[no].offs, 4);
	    if ((er = rd->memo->vtbl->getvalue(ClipMachineMemory, rd->memo, _rdd_uint((unsigned char *) memo), vp, __PROC__)))
	       return er;
	 }
      }
      else
      {
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup("!!! Memo file not opened");
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 24;
      }
      break;
   case 'N':
   case 'F':			/* Visual FoxPro type */
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      str = (char *) malloc(rd->fields[no].len + 1);
      str[rd->fields[no].len] = 0;
      memcpy(str, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
      vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _clip_strtod(str, &yy);
      free(str);
      vp->ClipType_t_of_ClipVar.len_of_ClipType = rd->fields[no].len;
      vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
      break;
   case 'D':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      memcpy(date, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
      if (!isdigit(date[0]))
      {
	 dd = 0;
      }
      else
      {
	 dd = (date[7] - '0');
	 dd += (date[6] - '0') * 10;
	 mm = (date[5] - '0');
	 mm += (date[4] - '0') * 10;
	 yy = (date[3] - '0');
	 yy += (date[2] - '0') * 10;
	 yy += (date[1] - '0') * 100;
	 yy += (date[0] - '0') * 1000;
      }
      if (dd)
	 vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate(dd, mm, yy);
      else
	 vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
      break;
   case 'V':
      {
	 if (rd->fields[no].len == 3)
	 {
	    unsigned char vd[4];

	    unsigned int vi;

	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    memcpy(vd + 1, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
	    *vd = 0;
	    if (vd[1] == ' ' || vd[1] == 0)
	       vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	    else
	    {
	       vi = _rdd_backuint(vd);
	       vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate((vi & 0x1f), (vi & 0x1e0) >> 5, vi >> 9);
	    }
	 }
	 else if (rd->fields[no].len == 4)
	 {
	    unsigned char vi[4];

	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    memcpy(vi, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
	    vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) _rdd_uint(vi);
	    vp->ClipType_t_of_ClipVar.len_of_ClipType = 11;
	    vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 }
	 else
	 {
	    unsigned char vlen[4];

	    if (rd->memo)
	    {
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	       vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       if (rd->indexing)
	       {
		  char *s;

		  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len - 6;
		  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
		  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
		  memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			 (char *) rd->record + rd->fields[no].offs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
		  while (!(*(--s)))
		     *s = ' ';
		  if (!rd->fields[no].binary)
		     loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  return 0;
	       }
	       memcpy(vlen, (char *) rd->record + rd->fields[no].offs + rd->fields[no].len - 2, 2);
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = _rdd_ushort(vlen);
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
	       if (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf <= rd->fields[no].len - 2)
	       {
		  memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record +
			 rd->fields[no].offs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	       }
	       else
	       {
		  memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len - 6);
		  memcpy(vlen, (char *) rd->record + rd->fields[no].offs + rd->fields[no].len - 6, 4);
		  if (rd->memo->vtbl->getvchar)
		  {
		     if ((er =
			  rd->memo->vtbl->getvchar(ClipMachineMemory, rd->memo,
						   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf -
						   (rd->fields[no].len - 6), _rdd_uint(vlen), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rd->fields[no].len - 6, __PROC__)))
			return er;
		  }
	       }
	    }
	    else
	    {
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup("!!! Memo file not opened");
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 24;
	    }
	    if (!rd->fields[no].binary)
	       loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 }
	 break;
      }
   case 'L':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      memcpy(&l, (char *) rd->record + rd->fields[no].offs, 1);
      vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l == 'T' ? 1 : 0);
      break;
   case 'X':
      {
	 memset(vp, 0, sizeof(ClipVar));
	 switch (((char *) rd->record)[rd->fields[no].offs])
	 {
	 case 'N':
	    {
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	       memcpy(&vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, (char *) rd->record + rd->fields[no].offs + 2, 8);
	       vp->ClipType_t_of_ClipVar.len_of_ClipType = ((char *) rd->record)[rd->fields[no].offs + 1];
	       vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	       break;
	    }
	 case 'C':
	    {
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ((unsigned char *) rd->record)[rd->fields[no].offs + 1];
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
		      (char *) rd->record + rd->fields[no].offs + 2, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
	       if (!rd->fields[no].binary)
		  loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	       break;
	    }
	 case 'D':
	    {
	       unsigned char vd[4];

	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	       memcpy(vd + 1, (char *) rd->record + rd->fields[no].offs + 2, 3);
	       *vd = 0;
	       if (vd[1] == ' ' || vd[1] == 0)
		  vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	       else
		  vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _rdd_backuint(vd) + 0x15d971;
	       break;
	    }
	 case 'T':
	    {
	       char t[8];

	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	       memcpy(t, (char *) rd->record + rd->fields[no].offs + 2, 8);
	       vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_uint((unsigned char *) t);
	       vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_uint((unsigned char *) (t + 4));
	       break;
	    }
	 case 'L':
	    {
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	       vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (((char *) rd->record)[rd->fields[no].offs + 2] == 'T') ? 1 : 0;
	       break;
	    }
	 default:
	    break;
	 }
	 break;
      }
/* Visual FoxPro types */
   case 'I':
      {
	 unsigned char u4[4];

	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 memcpy(u4, (char *) rd->record + rd->fields[no].offs, 4);
	 vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) ((int) _rdd_uint(u4));
	 vp->ClipType_t_of_ClipVar.len_of_ClipType = 11;
	 vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 break;
      }
   case 'B':
   case 'Y':
      {
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 memcpy(&vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, (char *) rd->record + rd->fields[no].offs, 8);
	 if (rd->fields[no].type == 'Y')
	    vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) (*(long long *) &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar) / 10000;
	 vp->ClipType_t_of_ClipVar.len_of_ClipType = 20;
	 vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	 break;
      }
   case 'T':
      {
	 char t[8];

	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 memcpy(t, (char *) rd->record + rd->fields[no].offs, 8);
	 vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_uint((unsigned char *) t);
	 vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_uint((unsigned char *) (t + 4));
	 break;
      }
   }
   return 0;
}
