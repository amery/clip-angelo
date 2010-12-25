int
clip___DBCOPYRAW(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__RDDCOPY";

   int destwa = _clip_parni(ClipMachineMemory, 1);

   ClipVar *bfor = _clip_spar(ClipMachineMemory, 2);

   ClipVar *bwhile = _clip_spar(ClipMachineMemory, 3);

   int next = _clip_parni(ClipMachineMemory, 4);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   DBWorkArea *dwa;

   void *buf = NULL;

   ClipVar v, *vp;

   int cnt = 0, er;

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   dwa = get_area(ClipMachineMemory, destwa, 0, 0);
   CHECKWA(dwa);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);

   if (bfor && bfor->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      bfor = NULL;
   if (bwhile && bwhile->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      bwhile = NULL;

   buf = malloc(wa->rd->recsize);
   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (dwa->rd->vtbl->_wlock(ClipMachineMemory, dwa->rd, __PROC__))
      goto err_unlock;
   while (!wa->rd->eof)
   {
      if (bwhile)
      {
	 if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, bwhile, &v, 0)))
	    goto err_unlock;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	 {
	    er = rdd_err(ClipMachineMemory, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	    goto err_unlock;
	 }
	 if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 {
	    _clip_destroy(ClipMachineMemory, &v);
	    break;
	 }
	 _clip_destroy(ClipMachineMemory, &v);
      }
      if (bfor)
      {
	 if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, bfor, &v, 0)))
	    goto err_unlock;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	 {
	    er = rdd_err(ClipMachineMemory, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	    goto err_unlock;
	 }
	 if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 {
	    _clip_destroy(ClipMachineMemory, &v);
	    if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
	       goto err_unlock;
	    continue;
	 }
	 _clip_destroy(ClipMachineMemory, &v);
      }
      if ((er = rdd_rawread(ClipMachineMemory, wa->rd, buf, __PROC__)))
	 goto err_unlock;
      if ((er = rdd_rawwrite(ClipMachineMemory, dwa->rd, buf, 1, __PROC__)))
	 goto err_unlock;
      if (++cnt >= next)
	 break;
      if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
	 goto err_unlock;
   }
   if (wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__))
      goto err_unlock;
   if (dwa->rd->vtbl->_ulock(ClipMachineMemory, dwa->rd, __PROC__))
      goto err;
   free(buf);
   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   dwa->rd->vtbl->_ulock(ClipMachineMemory, dwa->rd, __PROC__);
 err:
   if (buf)
      free(buf);
   return er;
}
