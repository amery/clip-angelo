int
clip_HS_INDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_INDEX";

   const char *fname = _clip_parc(ClipMachineMemory, 1);

   const char *expr = _clip_parc(ClipMachineMemory, 2);

/*	int mode = _clip_parni(ClipMachineMemory,4);*/
   int pagesize = _clip_parni(ClipMachineMemory, 5) * 1024;

   int lcase = _clip_parl(ClipMachineMemory, 6);

   int filtset = _clip_parni(ClipMachineMemory, 7);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   char *tmp = NULL;

   unsigned int oldrecno;

   unsigned int lastrec;

   int oldbof, oldeof;

   ClipVar v, *vp;

   ClipVar block;

   unsigned int rno;

   HIPER *hs;

   int er;

   memset(&v, 0, sizeof(ClipVar));
   memset(&block, 0, sizeof(ClipVar));
   _clip_retni(ClipMachineMemory, -1);
   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      pagesize = getpagesize();
   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)
      lcase = 1;
   if (_clip_parinfo(ClipMachineMemory, 7) == UNDEF_type_of_ClipVarType)
      filtset = 1;

   if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   if ((er = hs_create(ClipMachineMemory, fname, pagesize, lcase, filtset, &hs, __PROC__)))
      goto err_unlock;
   oldrecno = wa->rd->recno;
   oldbof = wa->rd->bof;
   oldeof = wa->rd->eof;
   oldbof = oldeof = 0;
   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, (int *) &lastrec, __PROC__)))
      goto err_unlock;
   tmp = malloc(strlen(expr) + 5);
   sprintf(tmp, "{||%s}", expr);
   if ((er = _clip_eval_macro(ClipMachineMemory, tmp, strlen(tmp), &block)))
      goto err_unlock;
   memset(&v, 0, sizeof(ClipVar));
   for (wa->rd->recno = 1; wa->rd->recno <= lastrec; wa->rd->recno++)
   {
      if ((er = rdd_childs(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err_unlock;
      if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, &block, &v, 0)))
	 goto err_unlock;
      vp = _clip_vptr(&v);
      if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	 goto err_unlock;
      }
      if ((er =
	   hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &rno, __PROC__)))
	 goto err_unlock;
      _clip_destroy(ClipMachineMemory, &v);
   }
   free(tmp);
   tmp = NULL;
   wa->rd->bof = oldbof;
   wa->rd->eof = oldeof;
   wa->rd->recno = oldrecno;
   _clip_destroy(ClipMachineMemory, &block);
   if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, (void *) hs, _C_ITEM_TYPE_HIPER, destroy_hiper));
   hs->map = calloc(1, hs->lastrec ? (hs->lastrec + 31) >> 3 : 4);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (tmp)
      free(tmp);
   if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      _clip_destroy(ClipMachineMemory, &v);
   if (block.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      _clip_destroy(ClipMachineMemory, &block);
   return er;
}
