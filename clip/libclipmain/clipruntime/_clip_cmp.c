CLIP_DLLEXPORT int
_clip_cmp(ClipMachine * ClipMachineMemory, ClipVar * lval, ClipVar * rval, int *res, int bytype)
{
   ClipVar *Rval = rval, *Lval = lval;

   int ltype, rtype;

   int l = 0;

   Rval = _clip_vptr(Rval);
   Lval = _clip_vptr(Lval);

   ltype = _clip_type(Lval), rtype = _clip_type(Rval);

   if (bytype && ltype != rtype)
   {
      int wl, wr;

      wl = type_weight(ltype);
      wr = type_weight(rtype);
      if (wl < wr)
	 l = -1;
      else if (wl > wr)
	 l = 1;
      else
	 l = 0;
   }
   else if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
   {
      int rrlen = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      int rlen = rrlen;

      int lllen = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      int llen = lllen;

      char *s;

      for (s = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rlen - 1; rlen; --rlen, --s)
	 if (*s != ' ' /* && *s != '\n' */ )
	    break;

      if (!(ClipMachineMemory->flags & EXACT_FLAG))
      {				/* SET EXACT OFF - empty rstr, len must match for equal */
	 if (rrlen == 0)
	    l = 0;
#if 1
	 else if (lllen == 0 && rrlen > 0)
	    l = -1;
	 else			/* if ( rrlen > lllen ) */
	 {
	    if (lllen > rrlen)
	       lllen = rrlen;
	    l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, lllen, rrlen);
	 }
#else
	 else if (llen < rrlen)
	 {
	    l = _clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, llen);
	    if (!l)
	       l = -1;
	 }
	 else
	    l = _clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, rrlen);
#endif
      }
      else
	 /* SET EXACT ON - all but trailing spaces */
      {
	 if (llen == 0 && rlen > 0 /* llen > rlen */ )
	    l = -1;
	 else
	 {
	    for (s = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rlen - 1; rlen && *s == ' '; --rlen, --s)
	       ;
	    for (s = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen - 1; llen && *s == ' '; --llen, --s)
	       ;
	    if (rlen == 0 && llen > 0)
	       l = 1;
	    else if (rlen > 0 && llen == 0)
	       l = -1;
	    else
	       l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, llen, rlen);
	 }
      }
   }
   else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
   {
      l = (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar <
	   Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar ? -1 : (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar > Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar ? 1 : 0));
   }
   else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
   {
      l = (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar <
	   Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar ? -1 : (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar >
										 Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar ? 1 : 0));
      if (l == 0)
	 l = (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar <
	      Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar ? -1 : (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar >
										  Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar ? 1 : 0));
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
	 /*
	    l = (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar < Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar ? -1 : (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >
	    Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar ? 1 : 0));
	  */
      }
      else
      {
	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	 {
	    rational *tmp;

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
   }
   else if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
   {
      if (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && !Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 l = 1;
      else if (!Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 l = -1;
      else
	 l = 0;
   }
   else if (ltype == ARRAY_type_of_ClipVarType && rtype == ARRAY_type_of_ClipVarType)
   {
      l = (Lval != Rval);
   }
   else if (ltype == MAP_type_of_ClipVarType && rtype == MAP_type_of_ClipVarType)
   {
      l = (Lval != Rval);
   }
   else if (ltype == OBJECT_type_of_ClipVarType && rtype == OBJECT_type_of_ClipVarType)
   {
      l = (Lval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar != Rval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar);
   }
   else if (ltype == CCODE_type_of_ClipVarType && rtype == CCODE_type_of_ClipVarType)
   {
      l = (Lval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar != Rval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar);
   }
   else if (ltype == PCODE_type_of_ClipVarType && rtype == PCODE_type_of_ClipVarType)
   {
      l = (Lval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar != Rval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);
   }
   else if ((ltype == PCODE_type_of_ClipVarType && rtype == CCODE_type_of_ClipVarType) || (ltype == CCODE_type_of_ClipVarType && rtype == PCODE_type_of_ClipVarType))
   {
      l = 0;
   }
   else if (ltype == UNDEF_type_of_ClipVarType)
   {
      l = (rtype == UNDEF_type_of_ClipVarType ? 0 : -1);
   }
   else if (rtype == UNDEF_type_of_ClipVarType)
   {
      l = (ltype == UNDEF_type_of_ClipVarType ? 0 : 1);
   }
   else
   {
      return _clip_trap_operation(ClipMachineMemory, __file__, __LINE__, "==", Lval);
      /*return _clip_call_errblock(ClipMachineMemory, 1); */
   }

   *res = l;
   return 0;
}
