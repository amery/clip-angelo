static int
do_assign(ClipMachine * ClipMachineMemory, ClipVar * lval, ClipVar * rval, int op)
{
   int ret = 0;

   int l;

   ClipVar *Rval = rval, *Lval = lval;

   int ltype, rtype;

   Rval = _clip_vptr(Rval);
   Lval = _clip_vptr(Lval);

   ltype = _clip_type(Lval);
   rtype = _clip_type(Rval);

   if (ltype == MAP_type_of_ClipVarType)
   {
      int ind;

      long hash = 0;

      switch (op)
      {
      case '+':
	 hash = HASH_operator_add;
	 break;
      case '-':
	 hash = HASH_operator_sub;
	 break;
      case '*':
	 hash = HASH_operator_mul;
	 break;
      case '/':
	 hash = HASH_operator_div;
	 break;
      case '%':
	 hash = HASH_operator_mod;
	 break;
      case '^':
	 hash = HASH_operator_pow;
	 break;
      case '|':
	 hash = HASH_operator_or;
	 break;
      case '&':
	 hash = HASH_operator_and;
	 break;
      case '$':
	 hash = HASH_operator_in;
	 break;
      case 'e':
	 hash = HASH_operator_eq;
	 break;
      case 'E':
	 hash = HASH_operator_eeq;
	 break;
      case 'N':
	 hash = HASH_operator_neq;
	 break;
      case '<':
	 hash = HASH_operator_lt;
	 break;
      case '>':
	 hash = HASH_operator_gt;
	 break;
      case 'L':
	 hash = HASH_operator_le;
	 break;
      case 'G':
	 hash = HASH_operator_ge;
	 break;
      }

      if (hash)
      {
	 ClipVar *vp = Lval;

	 /* try run `destroy` member */
	 if (search_map(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, hash, &ind))
	 {
	    ClipVar *ep;

	    int type;

	    ep = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	    type = _clip_type(ep);

	    if (type == PCODE_type_of_ClipVarType || type == CCODE_type_of_ClipVarType)
	    {
	       int i;

	       ClipVar stack[2], res;

	       memset(stack, 0, sizeof(stack));
	       memset(&res, 0, sizeof(res));

	       stack[0].ClipType_t_of_ClipVar = vp->ClipType_t_of_ClipVar;
	       stack[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	       stack[0].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	       vp->ClipType_t_of_ClipVar.count_of_ClipType++;

	       _clip_clone(ClipMachineMemory, stack + 1, rval);

	       ret = _clip_eval(ClipMachineMemory, _clip_vptr(ep), 2, stack, &res);

	       _clip_clone(ClipMachineMemory, lval, &res);

	       for (i = 0; i < 2; i++)
		  _clip_destroy(ClipMachineMemory, stack + i);

	       _clip_destroy(ClipMachineMemory, &res);
	    }
	    return ret;
	 }
      }
   }

   switch (op)
   {
   case '+':
      if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
      {
	 int len = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 if (Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
	 {
	    char *s = (char *) malloc(len + 1);

	    Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    memcpy(s, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
	 }
	 else
	    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) realloc(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + 1);
	 memcpy(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf +
		Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
		Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
	 Lval->ClipType_t_of_ClipVar.len_of_ClipType = Rval->ClipType_t_of_ClipVar.len_of_ClipType;
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 _clip_math_operation(ClipMachineMemory, op, Lval, Rval);
      }
      else if (ltype == DATE_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
      {
	 double tmp;

	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    tmp = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
	 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += tmp;
      }
      else if (ltype == DATETIME_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 long tmp;

	 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar += tmp / (24 * 60 * 60);
	 Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar += (tmp % (24 * 60 * 60)) * 1000;
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
      {
	 long tmp;

	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp = (long) rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    tmp = (long) Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	 Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar + tmp / (24 * 60 * 60);
	 Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar + (tmp % (24 * 60 * 60)) * 1000;
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "+", Lval);
	 /*return 1; */
      }
      break;
   case '-':
      if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
      {
	 int llen = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 int len, slen = 0;

	 char *s;

	 for (s = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen - 1; llen; --llen, --s, ++slen)
	    if (*s != ' ' && *s != '\t')
	       break;
	 len = llen + Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 if (Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
	 {
	    char *s = (char *) malloc(len + 1);

	    Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    memcpy(s, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
	 }
	 else
	    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) realloc(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + slen + 1);
	 memcpy(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen,
		Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 memset(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len, ' ', slen);
	 Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[len + slen] = 0;
	 Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len + slen;
	 Lval->ClipType_t_of_ClipVar.len_of_ClipType = Rval->ClipType_t_of_ClipVar.len_of_ClipType;
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 _clip_math_operation(ClipMachineMemory, op, Lval, Rval);
      }
      else if (ltype == DATE_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
      else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
      {
	 long tmp = (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar - Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar) * 24 * 60 * 60;

	 tmp += (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar - Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar) / 1000;

	 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = tmp;
      }
      else if (ltype == DATETIME_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 long tmp;

	 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar -= tmp / (24 * 60 * 60);
	 Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar -= (tmp % (24 * 60 * 60)) * 1000;
	 if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar < 0)
	 {
	    Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar--;
	    Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = (24 * 60 * 60 * 1000) + Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
	 }
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
      {
	 double tmp;

	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    tmp = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
	 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= tmp;
      }
      else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
      {
	 Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar - Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
	 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
	 Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "-", Lval);
	 /*return 1; */
      }
      break;
   case '*':
      if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 _clip_math_operation(ClipMachineMemory, op, Lval, Rval);
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "*", Lval);
	 /*return 1; */
      }
      break;
   case '/':
      if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 _clip_math_operation(ClipMachineMemory, op, Lval, Rval);
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "/", Lval);
	 /*return 1; */
      }
      break;
   case '%':
      if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType && !Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fmod(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
	    Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	    Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 }
	 else
	 {
	    rational *tmp = 0;

	    if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       tmp = rational_fmod(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    }
	    else
	    {
	       if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	       {
		  rational *tmp1;

		  tmp1 = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  tmp = rational_fmod(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp1);
		  rational_destroy(tmp1);
	       }
	       else
	       {
		  rational *tmp1;

		  tmp1 = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  tmp = rational_fmod(tmp1, Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		  rational_destroy(tmp1);
	       }
	    }
	    rational_destroy(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = tmp;
	 }

      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "%", Lval);
	 /*return 1; */
      }
      break;
   case '^':
      if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    long tmp;

	    if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	       tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    else
	       tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    rational_powa(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
	 }
	 else
	 {
	    Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = pow(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    if (!finite(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar))
	       Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	    Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
	    Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 }
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "**", Lval);
	 /*return 1; */
      }
      break;
   case '$':
      if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
      {
	 const char *s = _clip_strstr(Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				      Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
				      Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				      Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	 _clip_destroy(ClipMachineMemory, lval);
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (s ? 1 : 0);
	 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      }
      else if (rtype == MAP_type_of_ClipVarType)
      {
	 int ind, r;

	 long no;

	 switch (ltype)
	 {
	 case NUMERIC_type_of_ClipVarType:
	    if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	       no = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    else
	       no = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    break;
	 case CHARACTER_type_of_ClipVarType:
	    /*no = _clip_casehashbytes(0, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf); */
	    no = _clip_hashbytes(0, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    break;
	 default:
	    goto inv_in_arg;
	 }

	 r = search_map(Rval->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, Rval->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, &ind);
	 _clip_destroy(ClipMachineMemory, lval);
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (r ? 1 : 0);
	 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      }
      else if (rtype == ARRAY_type_of_ClipVarType)
      {
	 int i, c, r, res = 0;

	 c = Rval->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

	 for (i = 0; i < c; ++i)
	 {
	    ClipVar *vp = Rval->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	    r = _clip_cmp(ClipMachineMemory, Lval, vp, &res, 1);
	    if (r)
	       continue;
	    if (res)
	       break;
	 }
	 _clip_destroy(ClipMachineMemory, lval);
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (!res ? 1 : 0);
	 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      }
      else
      {
       inv_in_arg:
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "$", Lval);
	 /*return 1; */
      }
      break;
   case 'E':
   case 'e':
   case 'N':
#if 1				/* uri added : oper != work as =, not == */
      if ((op == 'E') && (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType))
#else
      if ((op == 'E' || op == 'N') && (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType))
#endif
#if 1
	 /*l = !strcmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf); */
      {
	 int ll, lr;

	 ll = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 lr = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 if (ll != lr)
	    l = 0;
	 else
	    l = !memcmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ll);
      }
#else
/*Rust begin */
      {
	 int len =
	    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len <
	    Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len ? Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf : Rval->
	    ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	 l = !_clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
      }
/*Rust end */
#endif
      else
      {
	 if ((ret = _clip_cmp(ClipMachineMemory, lval, rval, &l, 0)))
	    return ret;
	 l = !l;
      }

      _clip_destroy(ClipMachineMemory, lval);
      switch (op)
      {
      case 'E':
      case 'e':
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
	 break;
      default:
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = !l;
	 break;
      }
      lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      break;
   case '|':
      if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
      {
	 l = (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar || Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "OR", lval);
	 /*return 1; */
      }
      lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
      lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      break;
   case '&':
      if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
      {
	 l = (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "AND", lval);
	 /*return 1; */
      }
      lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
      lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      break;
   case '<':
   case '>':
   case 'L':
   case 'G':
      if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
      {
	 l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			    Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
#if 1				/* added 20030118 for test '? "" <="0"' */
	 ret = _clip_cmp(ClipMachineMemory, lval, rval, &l, 0);
#endif
	 //printf("\nl=%d,ret=%d\n",l,ret);
	 _clip_destroy(ClipMachineMemory, lval);
      }
      else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
      {
	 if (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar < Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
	    l = -1;
	 else if (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar > Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
	    l = 1;
	 else
	    l = 0;
      }
      else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
      {
	 if (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar)
	    l = -1;
	 else if (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar)
	    l = 1;
	 else
	    l = 0;
	 if (l == 0)
	 {
	    if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar)
	       l = -1;
	    else if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar)
	       l = 1;
	    else
	       l = 0;
	 }
      }
      else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
	 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType && !Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    double diff;

	    diff = fabs(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    if (diff < (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar * MIN_NUMERIC_DIFF))
	       l = 0;
	    else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar < Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
	       l = -1;
	    else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar > Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
	       l = 1;
	    else
	       l = 0;
	 }
	 else
	 {
	    if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	       l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    else
	    {
	       rational *tmp = NULL;

	       if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	       {
		  tmp = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
	       }
	       else
	       {
		  tmp = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  l = rational_cmp(tmp, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       }
	       rational_destroy(tmp);
	    }
	 }
	 _clip_destroy(ClipMachineMemory, lval);
      }
      else if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
      {
	 if (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && !Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    l = -1;
	 else if (!Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    l = 1;
	 else
	    l = 0;
      }
      else if (ltype == OBJECT_type_of_ClipVarType && rtype == OBJECT_type_of_ClipVarType)
      {
	 l = Lval->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->cmp_of_ClipObjRtti(ClipMachineMemory,
										  Lval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, Lval->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, Rval);
	 _clip_destroy(ClipMachineMemory, lval);
      }
      else
      {
	 return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "==", lval);
	 /*return 1; */
      }
      switch (op)
      {
      case 'L':
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l <= 0 ? 1 : 0);
	 break;
      case 'G':
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l >= 0 ? 1 : 0);
	 break;
      case '>':
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l > 0 ? 1 : 0);
	 break;
      case '<':
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l < 0 ? 1 : 0);
	 break;
      default:
	 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
      }
      lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      break;
   }

   return ret;
}
