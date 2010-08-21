int
dbf_setvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, int binary, const char *__PROC__)
{
   int er;

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if (!rd->data[no])
   {
      rd->data[no] = calloc(1, sizeof(ClipVar));
      if ((er = dbf_getvalue(ClipMachineMemory, rd, no, rd->data[no], __PROC__)))
	 return er;
   }
   rd->updated = rd->changed = 1;
   if (rd->fields[no].nullable)
   {
      int nbyte = no >> 3;

      unsigned char byte;

      memcpy(&byte, rd->record + rd->nulloffs + nbyte, 1);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
	 byte |= (1 << (no - (nbyte << 3)));
      else
	 byte &= ~(1 << (no - (nbyte << 3)));
      memcpy(rd->record + rd->nulloffs + nbyte, &byte, 1);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
	 return 0;
   }
   switch (rd->fields[no].type)
   {
   case 'C':
      {
	 char *str;

	 int l;

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    goto err;
	 str = malloc(rd->fields[no].len);
	 l = min(rd->fields[no].len, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	 memcpy(str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	 if (!rd->fields[no].binary)
	    loc_write(rd->loc, (unsigned char *) str, l);
	 memset(str + l, ' ', rd->fields[no].len - l);
	 memcpy(rd->record + rd->fields[no].offs, str, rd->fields[no].len);
	 free(str);
	 break;
      }
   case 'M':
   case 'P':
   case 'G':
      {
	 char memoid[11];

	 int memo;

	 if (!rd->memo)
	    return rdd_err(ClipMachineMemory, EG_WRITE, 0, __FILE__, __LINE__, __PROC__, er_nomemo);
	 if (rd->fields[no].type == 'P' || rd->fields[no].type == 'G')
	    binary = 1;
	 if (rd->sig != 0x30)
	 {			/* Not VFP */
	    memcpy(memoid, rd->record + rd->fields[no].offs, 10);
	    memoid[10] = 0;
	    memo = atoi(memoid);
	    if ((er = rd->memo->vtbl->setvalue(ClipMachineMemory, rd->memo, &memo, vp, binary, __PROC__)))
	       return er;
	    snprintf(memoid, sizeof(memoid), "%10d", memo);
	    memcpy(rd->record + rd->fields[no].offs, memoid, 10);
	 }
	 else
	 {
	    memcpy(memoid, rd->record + rd->fields[no].offs, 4);
	    memo = _rdd_uint((unsigned char *) memoid);
	    if ((er = rd->memo->vtbl->setvalue(ClipMachineMemory, rd->memo, &memo, vp, binary, __PROC__)))
	       return er;
	    _rdd_put_uint((unsigned char *) memoid, memo);
	    memcpy(rd->record + rd->fields[no].offs, memoid, 4);
	 }
	 break;
      }
   case 'N':
   case 'F':			/* Visual FoxPro type */
      {
	 char *n;

	 int len = rd->fields[no].len;

	 int dec = rd->fields[no].dec;

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	    goto err;
	 n = calloc(1, len + 1);
	 if (_clip_dtostr(n, len, dec, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0))
	 {
	    char buf[PATH_MAX];

	    snprintf(buf, sizeof(buf), "%s: %s", rd->path, rd->fields[no].name);
	    free(n);
	    return rdd_err(ClipMachineMemory, EG_DATAWIDTH, 0, __FILE__, __LINE__, buf, er_baddata);
	 }
	 memcpy(rd->record + rd->fields[no].offs, n, len);
	 free(n);
	 break;
      }
   case 'D':
      {
	 char d[] = "00000000";

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
	    goto err;
	 if (vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
	 {
	    _clip_datetostr(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, d);
	 }
	 memcpy(rd->record + rd->fields[no].offs, d, 8);
	 break;
      }
   case 'L':
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	 goto err;
      memcpy(rd->record + rd->fields[no].offs, vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? "T" : "F", 1);
      break;
   case 'V':
      {
	 if (rd->fields[no].len == 3)
	 {
	    unsigned char vd[4];

	    unsigned int vi;

	    int dd, mm, yy, ww;

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
	       goto err;

	    _clip_cdate(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	    vi = (yy << 9) + (mm << 5) + dd;

	    _rdd_put_backuint(vd, vi);
	    memcpy(rd->record + rd->fields[no].offs, vd + 1, 3);
	 }
	 else if (rd->fields[no].len == 4)
	 {
	    unsigned char vi[4];

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	       goto err;
	    _rdd_put_uint(vi, (int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    memcpy(rd->record + rd->fields[no].offs, vi, 4);
	 }
	 else
	 {
	    unsigned char vlen[4];

	    char *str;

	    unsigned short int oldlen;

	    unsigned int memoid = 0;

	    if (!rd->memo)
	       return rdd_err(ClipMachineMemory, EG_WRITE, 0, __FILE__, __LINE__, __PROC__, er_nomemo);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	       goto err;
	    memcpy(vlen, rd->record + rd->fields[no].offs + rd->fields[no].len - 2, 2);
	    oldlen = max((int) _rdd_ushort(vlen) - (rd->fields[no].len - 6), 0);
	    if (oldlen > rd->fields[no].len - 6)
	    {
	       memcpy(vlen, rd->record + rd->fields[no].offs + rd->fields[no].len - 6, 4);
	       memoid = _rdd_uint(vlen);
	    }
	    _rdd_put_ushort(vlen, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    memcpy(rd->record + rd->fields[no].offs + rd->fields[no].len - 2, vlen, 2);
	    str = calloc(1, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    memcpy(str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    if (!rd->fields[no].binary)
	       loc_write(rd->loc, (unsigned char *) str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    if (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf <= rd->fields[no].len - 2)
	    {
	       memcpy(rd->record + rd->fields[no].offs, str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    }
	    else
	    {
	       memcpy(rd->record + rd->fields[no].offs, str, rd->fields[no].len - 6);
	       if (rd->memo->vtbl->setvchar)
	       {
		  if ((er = rd->memo->vtbl->setvchar(ClipMachineMemory, rd->memo,
						     vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf -
						     (rd->fields[no].len - 6), oldlen, &memoid, str + rd->fields[no].len - 6, __PROC__)))
		  {
		     free(str);
		     return er;
		  }
	       }
	       _rdd_put_uint(vlen, memoid);
	       memcpy(rd->record + rd->fields[no].offs + rd->fields[no].len - 6, vlen, 4);
	    }
	    free(str);
	 }
	 break;
      }
   case 'X':
      {
	 memset((char *) rd->record + rd->fields[no].offs, 0, rd->fields[no].len);
	 switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	 {
	 case NUMERIC_type_of_ClipVarType:
	    {
	       ((char *) rd->record)[rd->fields[no].offs] = 'N';
	       ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = vp->ClipType_t_of_ClipVar.len_of_ClipType;
	       memcpy(rd->record + rd->fields[no].offs + 2, &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 8);
	       break;
	    }
	 case CHARACTER_type_of_ClipVarType:
	    {
	       unsigned int len = (unsigned char) min(rd->fields[no].len - 2,
						      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	       ((char *) rd->record)[rd->fields[no].offs] = 'C';
	       ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = len;
	       memcpy((char *) rd->record + rd->fields[no].offs + 2, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	       if (!rd->fields[no].binary)
		  loc_write(rd->loc, (unsigned char *) rd->record + rd->fields[no].offs + 2, len);
	       break;
	    }
	 case DATE_type_of_ClipVarType:
	    {
	       unsigned char vd[4];

	       ((char *) rd->record)[rd->fields[no].offs] = 'D';
	       ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 3;
	       _rdd_put_backuint(vd, vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar - 0x15d971);
	       memcpy(rd->record + rd->fields[no].offs + 2, vd + 1, 3);
	       break;
	    }
	 case DATETIME_type_of_ClipVarType:
	    {
	       char t[8];

	       ((char *) rd->record)[rd->fields[no].offs] = 'T';
	       ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 8;
	       _rdd_put_uint((unsigned char *) t, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
	       _rdd_put_uint((unsigned char *) (t + 4), vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
	       memcpy((char *) rd->record + rd->fields[no].offs + 2, t, 8);
	       break;
	    }
	 case LOGICAL_type_of_ClipVarType:
	    {
	       ((char *) rd->record)[rd->fields[no].offs] = 'L';
	       ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 1;
	       ((char *) rd->record)[rd->fields[no].offs + 2] = (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F');
	       break;
	    }
	 case UNDEF_type_of_ClipVarType:
	    break;
	 default:
	    goto err;
	 }
	 break;
      }
/* Visual FoxPro types */
   case 'I':
      {
	 unsigned char u4[4];

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	    goto err;
	 _rdd_put_uint(u4, (int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 memcpy(rd->record + rd->fields[no].offs, u4, 4);
	 break;
      }
   case 'B':
   case 'Y':
      {
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	    goto err;
	 if (rd->fields[no].type == 'Y')
	 {
	    long long tmp;

	    double d = vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

#ifdef ARCH_i386
	    long long *lld = (long long *) &d;

	    (*lld)++;
#endif
	    tmp = d * 10000;
	    memcpy(rd->record + rd->fields[no].offs, &tmp, 8);
	 }
	 else
	    memcpy(rd->record + rd->fields[no].offs, &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 8);
	 break;
      }
   case 'T':
      {
	 char t[8];

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATETIME_type_of_ClipVarType)
	    goto err;
	 _rdd_put_uint((unsigned char *) t, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
	 _rdd_put_uint((unsigned char *) (t + 4), vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
	 memcpy((char *) rd->record + rd->fields[no].offs, t, 8);
	 break;
      }
   }
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_type);
}
