static unsigned int *
rm_prim(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, unsigned int *obm, const char *__PROC__)
{
   unsigned int *bm;

   int oper;

   char *lval;

   char *rval;

   char *p;

   if (!obm)
      bm = calloc(sizeof(unsigned int), bytes);
   else
      bm = obm;
   rd->curlex = rm_yylex(rd, 0);
   switch (rd->curlex)
   {
   case RM_NOT:
      {
	 unsigned int *tm;

	 ADDLEX("!");
	 tm = rm_prim(ClipMachineMemory, rd, fp, bytes, optimize, npseudo, pseudo, test, NULL, __PROC__);
	 if (!tm)
	 {
	    free(bm);
	    return NULL;
	 }
	 if (*optimize)
	    rm_not(bm, tm, bytes);
	 else
	 {
	    memcpy(bm, tm, bytes * 4);
	    free(tm);
	 }
	 return bm;
      }
   case RM_LP:
      ADDLEX("(");
      free(bm);
      bm = rm_expr(ClipMachineMemory, rd, fp, bytes, optimize, npseudo, pseudo, test, __PROC__);
      if (rd->curlex != RM_RP)
      {
	 free(bm);		// Right bracket expected
	 return NULL;
      }
      ADDLEX(")");
      break;
   case RM_WORD:
      {
	 ClipVar lb, rb, lv, rv;

	 char expr[1024];

	 int er;

	 p = lval = rm_alias2name(npseudo, pseudo, rd->yylval);
	 oper = rd->curlex = rm_yylex(rd, 0);
	 while (*p)
	 {
	    if (*p == RM_AL)
	       *p = '>';
	    p++;
	 }
	 if (oper != RM_EQU && oper != RM_EEQU && oper != RM_NEQU && oper != RM_LAR && oper != RM_LARE && oper != RM_LES && oper != RM_LESE)
	 {
	    rdd_expandmacro(rd->area, rd->rdhandle, lval, expr);
	    er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &lb);
	    if (er)
	    {
	       ClipMachineMemory->m6_error = 2219;
	       _clip_destroy(ClipMachineMemory, &lb);
	       free(lval);
	       free(bm);
	       return NULL;
	    }
	    ClipMachineMemory->noerrblock++;
	    er = _clip_eval(ClipMachineMemory, _clip_vptr(&lb), 0, NULL, &lv);
	    ClipMachineMemory->noerrblock--;
	    _clip_destroy(ClipMachineMemory, &lb);
	    _clip_destroy(ClipMachineMemory, &lv);
	    if (er)
	    {
	       ClipMachineMemory->m6_error = 2221;
	       ADDLEX(".T.");
	       if (!obm)
		  memset(bm, 0xff, bytes << 2);
	    }
	    else
	    {
	       ADDLEX(lval);
	       if (obm)
	       {
		  if (rm_intersectscope(ClipMachineMemory, rd, fp, bm, RM_EEQU, lval, ".T.", bytes, optimize, test, NULL, __PROC__))
		     return NULL;
	       }
	       else
	       {
		  if (rm_setscope(ClipMachineMemory, rd, fp, bm, RM_EEQU, lval, ".T.", bytes, optimize, test, NULL, __PROC__))
		     return NULL;
	       }
	    }
	    free(lval);
	    return bm;
	 }
	 rd->curlex = rm_yylex(rd, 0);
	 if (rd->curlex != RM_WORD)
	 {
	    rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Expression expected");
	    return NULL;
	 }
	 p = rval = rm_alias2name(npseudo, pseudo, rd->yylval);
	 while (*p)
	 {
	    if (*p == RM_AL)
	       *p = '>';
	    p++;
	 }
	 rdd_expandmacro(rd->area, rd->rdhandle, lval, expr);
	 if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &lb)))
	 {
	    ClipMachineMemory->m6_error = 2219;
	    _clip_destroy(ClipMachineMemory, &lb);
	    free(lval);
	    free(rval);
	    free(bm);
	    return NULL;
	 }
	 ClipMachineMemory->noerrblock++;
	 er = _clip_eval(ClipMachineMemory, _clip_vptr(&lb), 0, NULL, &lv);
	 ClipMachineMemory->noerrblock--;
	 _clip_destroy(ClipMachineMemory, &lb);
	 _clip_destroy(ClipMachineMemory, &lv);
	 if (!er)
	 {
	    rdd_expandmacro(rd->area, rd->rdhandle, rval, expr);
	    if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &rb)))
	    {
	       ClipMachineMemory->m6_error = 2219;
	       _clip_destroy(ClipMachineMemory, &rb);
	       free(lval);
	       free(rval);
	       free(bm);
	       return NULL;
	    }
	    ClipMachineMemory->noerrblock++;
	    er = _clip_eval(ClipMachineMemory, _clip_vptr(&rb), 0, NULL, &rv);
	    ClipMachineMemory->noerrblock--;
	    _clip_destroy(ClipMachineMemory, &rb);
	    _clip_destroy(ClipMachineMemory, &rv);
	 }
	 if (er)
	 {
	    ClipMachineMemory->m6_error = 2221;
	    ADDLEX(".T.");
	    if (!obm)
	       memset(bm, 0xff, bytes << 2);
	 }
	 else
	 {
	    int ic = 0;

	    if (obm)
	    {
	       if (rm_intersectscope(ClipMachineMemory, rd, fp, bm, oper, lval, rval, bytes, optimize, test, &ic, __PROC__))
		  return NULL;
	    }
	    else
	    {
	       if (rm_setscope(ClipMachineMemory, rd, fp, bm, oper, lval, rval, bytes, optimize, test, &ic, __PROC__))
		  return NULL;
	    }
	    if (ic)
	       ADDLEX("xupper(");
	    ADDLEX(lval);
	    if (ic)
	       ADDLEX(")");
	    switch (oper)
	    {
	    case RM_EQU:
	       ADDLEX("=");
	       break;
	    case RM_EEQU:
	       ADDLEX("==");
	       break;
	    case RM_NEQU:
	       ADDLEX("!=");
	       break;
	    case RM_LAR:
	       ADDLEX(">");
	       break;
	    case RM_LARE:
	       ADDLEX(">=");
	       break;
	    case RM_LES:
	       ADDLEX("<");
	       break;
	    case RM_LESE:
	       ADDLEX("<=");
	       break;
	    }
	    if (ic)
	       ADDLEX("xupper(");
	    ADDLEX(rval);
	    if (ic)
	       ADDLEX(")");
	 }
	 free(lval);
	 free(rval);
	 break;
      }
   default:
      return NULL;
   }
   rd->curlex = rm_yylex(rd, 0);
   return bm;
}
