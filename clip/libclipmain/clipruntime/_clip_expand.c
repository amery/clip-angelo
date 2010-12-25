CLIP_DLLEXPORT int
_clip_expand(ClipMachine * ClipMachineMemory, ClipBuf * dest, ClipBuf * src)
{
   char *s, *e, *beg, *end, *next;

   int dot;

   OutBuf buf;

   init_Buf(&buf);

   s = src->buf_of_ClipBuf;
   e = s + src->len_of_ClipBuf;
   for (; find_macro(s, e, &beg, &end, &next, &dot); s = next)
   {
      long hash;

      VarEntry *vp;

      ClipVar *vpp;

      int ch;

      putBuf_Buf(&buf, s, (beg - 1) - s);
      if (end == beg)
      {
	 putByte_Buf(&buf, '&');
	 if (dot)
	    putByte_Buf(&buf, '.');
	 continue;
      }

      if (*beg == '\\')
      {
	 ++beg;
	 if (end == beg)
	 {
	    putByte_Buf(&buf, '&');
	    putByte_Buf(&buf, '\\');
	    continue;
	 }

	 switch ((ch = *beg))
	 {
	 case 'x':
	 case 'X':
	    {
	       ++beg;
	       if ((end - beg) != 2)
	       {
		undo:
		  putByte_Buf(&buf, '&');
		  putByte_Buf(&buf, '\\');
		  while (beg < end)
		     putByte_Buf(&buf, *beg++);
		  continue;
	       }
	       else
	       {
		  unsigned char b1, b2;

		  b1 = hexdig(beg[0]);
		  b2 = hexdig(beg[1]);
		  putByte_Buf(&buf, b1 << 4 | b2);
	       }
	    }
	    continue;
	 case '0':
	 case '1':
	 case '2':
	 case '3':
	 case '4':
	 case '5':
	 case '6':
	 case '7':
	    {
	       unsigned char b1, b2, b3;

	       if ((end - beg) != 3)
		  goto undo;
	       b1 = octdig(beg[0]);
	       b2 = octdig(beg[1]);
	       b3 = octdig(beg[2]);
	       putByte_Buf(&buf, b1 << 6 | b2 << 3 | b3);
	    }
	    continue;
	 default:
	    if ((end - beg) != 1)
	       goto undo;
	    switch (ch)
	    {
	    case 'n':
	       putByte_Buf(&buf, '\n');
	       break;
	    case 'r':
	       putByte_Buf(&buf, '\r');
	       break;
	    case 't':
	       putByte_Buf(&buf, '\t');
	       break;
	    case 'v':
	       putByte_Buf(&buf, '\v');
	       break;
	    case 'a':
	       putByte_Buf(&buf, 7);
	       break;
	    case 'e':
	       putByte_Buf(&buf, 27);
	       break;
	    default:
	       putByte_Buf(&buf, ch);
	       break;
	    }
	    continue;
	 }
      }

      hash = _clip_casehashbytes(0, beg, end - beg);
      vp = fetch_var(ClipMachineMemory, hash);
#if 0
      if (!vp)
      {
	 _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "undefined macro-variable in string");
	 goto _trap;
      }
#endif
#if 0
      if (!(vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType & CHARACTER_type_of_ClipVarType))
      {
	 _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "non-character result of macro in string");
	 goto _trap;
      }
#endif
      if (vp && !(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG))
      {
	 vpp = _clip_vptr(&vp->ClipVar_var_of_VarEntry);
	 switch (vpp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	 {
	 case CHARACTER_type_of_ClipVarType:
	    putBuf_Buf(&buf, vpp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vpp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    break;
	 case NUMERIC_type_of_ClipVarType:
	    {
	       if (vpp->ClipType_t_of_ClipVar.memo_of_ClipType)
	       {
		  char *bf;

		  if (vpp->ClipType_t_of_ClipVar.len_of_ClipType)
		     bf = rational_toString(vpp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, vpp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
		  else
		     bf = rational_toString(vpp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMachineMemory->decimals, 0);
		  putBuf_Buf(&buf, bf, strlen(bf));
		  free(bf);
	       }
	       else
	       {
		  char bf[32];

		  if (vpp->ClipType_t_of_ClipVar.len_of_ClipType)
		     snprintf(bf, sizeof(bf), "%*.*f", vpp->ClipType_t_of_ClipVar.len_of_ClipType, vpp->ClipType_t_of_ClipVar.dec_of_ClipType, vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  else
		     snprintf(bf, sizeof(bf), "%g", vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  putBuf_Buf(&buf, bf, strlen(bf));
	       }
	    }
	    break;
	 case LOGICAL_type_of_ClipVarType:
	    if (vpp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	       putBuf_Buf(&buf, ".T.", 3);
	    else
	       putBuf_Buf(&buf, ".F.", 3);
	    break;
	 case DATE_type_of_ClipVarType:
	    {
	       int dd, mm, yy, ww;

	       char bf[32];

	       _clip_cdate(vpp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	       sprintf(bf, "%02d/%02d/%d", dd, mm, yy);
	       putBuf_Buf(&buf, bf, strlen(bf));
	    }
	    break;
	 case DATETIME_type_of_ClipVarType:
	    {
	       int len;

	       char *r = _clip_ttoc(ClipMachineMemory, vpp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar,
				    vpp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &len,
				    ClipMachineMemory->date_format, ClipMachineMemory->hours,
				    ClipMachineMemory->seconds);

	       putBuf_Buf(&buf, r, len);
	       free(r);
	    }
	    break;
	 default:
	    goto badtype;
	    break;
	 }
      }
      else
      {
       badtype:
	 putByte_Buf(&buf, '&');
	 while (beg < end)
	    putByte_Buf(&buf, *beg++);
	 if (dot)
	    putByte_Buf(&buf, '.');
      }
   }
   /* rest of string */
   if (s < e)
      putBuf_Buf(&buf, s, e - s);

   dest->len_of_ClipBuf = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   putByte_Buf(&buf, 0);
   dest->buf_of_ClipBuf = buf.buf_of_OutBuf;
   return 0;
}
