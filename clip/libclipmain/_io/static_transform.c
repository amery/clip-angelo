static void
transform(ClipMachine * ClipMachineMemory, ClipVar * vp, char *pict)
{
   char dbuf[32];

   char *templ = 0;

   int funclen = 0;

   int R_func = 0;

   int B_func = 0;

   int C_func = 0;

   int D_func = 0;

   int E_func = 0;

   int X_func = 0;

   int Z_func = 0;

   int BR_func = 0;

   int VO_func = 0;

   int n = 0;

   if (!vp)
   {
      _clip_printf(ClipMachineMemory, "");
      return;
   }
   vp = _clip_unptr(vp);

   if (pict)
   {
      if (pict[0] == '@')
      {
	 for (templ = pict + 1; *templ; ++templ, ++funclen)
	    if (*templ == ' ')
	    {
	       ++templ;
	       break;
	    }
	 if (!*templ)
	    templ = 0;
      }
      else
	 templ = pict;
   }

   if (funclen)
   {
      char *s, *e;

      for (s = pict + 1, e = s + funclen; *s && s < e; ++s)
	 switch (toupper(*s))
	 {
	 case 'R':
	    R_func = 1;
	    break;
	 case 'B':
	    B_func = 1;
	    break;
	 case 'C':
	    C_func = 1;
	    break;
	 case 'D':
	    D_func = 1;
	    break;
	 case 'E':
	    E_func = 1;
	    break;
	 case 'X':
	    X_func = 1;
	    break;
	 case 'Z':
	    Z_func = 1;
	    break;
	 case '(':
	 case ')':
	    BR_func = 1;
	    break;
	 case '!':
	    VO_func = 1;
	    break;
	 }

   }

   for (;;)
   {
      switch (vp->t.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 n = do_templ(vp->s.ClipBuf_str_of_ClipStrVar.buf, templ, ClipMachineMemory->buf, ClipMachineMemory->buflen, R_func, B_func, Z_func);
	 break;
      case NUMERIC_t:
	 {
	    double d;

	    if (vp->t.memo_of_ClipType)
	       d = rational_toDouble(vp->r.r);
	    else
	       d = vp->n.d;

	    _clip_dtos(d, dbuf, sizeof(dbuf), 0);
	    if (BR_func && d < 0)
	    {
	       d = -d;
	       _clip_dtos(d, dbuf, sizeof(dbuf), 0);
	       _clip_printf(ClipMachineMemory, "(%s)", dbuf);
	       strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	    }
	    else if (C_func && d > 0)
	    {
	       _clip_printf(ClipMachineMemory, "%s CR", dbuf);
	       strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	    }
	    else if (X_func && d < 0)
	    {
	       d = -d;
	       _clip_dtos(d, dbuf, sizeof(dbuf), 0);
	       _clip_printf(ClipMachineMemory, "%s DB", dbuf);
	       strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	    }

	    n = do_templ(dbuf, templ, ClipMachineMemory->buf, ClipMachineMemory->buflen, R_func, B_func, Z_func);
	 }
	 break;
      case LOGICAL_t:
	 if (vp->l.val)
	    strcpy(dbuf, "T");
	 else
	    strcpy(dbuf, "F");
	 n = do_templ(dbuf, templ, ClipMachineMemory->buf, ClipMachineMemory->buflen, R_func, B_func, Z_func);
	 break;
      case DATE_t:
	 {
	    long julian = vp->d.julian;

	    char *s;

	    int l;

	    if (D_func)
	       templ = ClipMachineMemory->date_format;
	    else if (E_func)
	       templ = "dd/mm/yy";
	    s = _clip_date_to_str(julian, templ);
	    l = strlen(s);
	    if (l >= ClipMachineMemory->buflen)
	       n = 0;
	    else
	    {
	       n = 1;
	       strncpy(ClipMachineMemory->buf, s, ClipMachineMemory->buflen);
	    }
	    free(s);
	 }
	 break;
      case OBJECT_t:
	 _clip_printf(ClipMachineMemory, "OBJECT");
	 return;
      case ARRAY_t:
	 _clip_printf(ClipMachineMemory, "ARRAY");
	 return;
      case PCODE_t:
      case CCODE_t:
	 _clip_printf(ClipMachineMemory, "CODE");
	 return;
      default:
	 break;
      }
      if (n)
	 break;
      ClipMachineMemory->buflen *= 2;
      ClipMachineMemory->buf = (char *) realloc(ClipMachineMemory->buf, ClipMachineMemory->buflen);
   }
}
