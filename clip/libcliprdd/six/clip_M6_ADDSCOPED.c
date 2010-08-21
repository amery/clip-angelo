int
clip_M6_ADDSCOPED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ADDSCOPED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   ClipVar *t = _clip_vptr(_clip_par(ClipMachineMemory, 2));

   ClipVar *b = _clip_vptr(_clip_par(ClipMachineMemory, 3));

   int ord = _clip_parni(ClipMachineMemory, 4) - 1;

   int opttype = _clip_parinfo(ClipMachineMemory, 5);

   int er, i, cnt = 0;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT3(5, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (!wa)
   {
      er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
      goto err;
   }
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (!fp->rmap)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      ord = wa->rd->curord;
   if (ord >= wa->rd->ords_opened)
      ord = -1;
   if (ord == -1)
   {
      er = rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);
      goto err;
   }

   if (t->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      t = NULL;
   if (b->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      b = NULL;

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = wa->rd->orders[ord]->vtbl->setscope(ClipMachineMemory, wa->rd, wa->rd->orders[ord], t, b, fp->rmap, fp->size, 0, __PROC__)))
      goto err_unlock;
   if (opttype)
   {
      int found;

      int oldrecno = wa->rd->recno;

      int oldbof = wa->rd->bof;

      int oldeof = wa->rd->eof;

      int words = (fp->size >> 5) + 1;

      int *tmap = calloc(4, words);

      if (opttype == CHARACTER_type_of_ClipVarType)
      {
	 if ((er = rdd_wildseek(ClipMachineMemory, wa->rd, _clip_parc(ClipMachineMemory, 5), 1, 0, &found, __PROC__)))
	    goto err_unlock;
	 while (!wa->rd->eof)
	 {
	    _rm_setbit(tmap, fp->size, wa->rd->recno);
	    if ((er = rdd_wildseek(ClipMachineMemory, wa->rd, _clip_parc(ClipMachineMemory, 5), 1, 1, &found, __PROC__)))
	       goto err_unlock;
	 }
      }
      else if ((opttype == CCODE_type_of_ClipVarType) || (opttype == PCODE_type_of_ClipVarType))
      {
	 if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
	    goto err_unlock;
	 while (!wa->rd->eof)
	 {
	    if ((er = rdd_seekeval(ClipMachineMemory, wa->rd, _clip_spar(ClipMachineMemory, 5), &found, __PROC__)))
	       goto err_unlock;
	    if (found)
	       _rm_setbit(tmap, fp->size, wa->rd->recno);
	    else
	       break;
	    if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
	       goto err_unlock;
	 }
      }
      if ((er = wa->rd->vtbl->rawgo(ClipMachineMemory, wa->rd, oldrecno, 0, __PROC__)))
	 goto err_unlock;
      wa->rd->bof = oldbof;
      wa->rd->eof = oldeof;
      for (i = 0; i < words; i++)
	 fp->rmap[i] = fp->rmap[i] & tmap[i];
   }
   UNLOCK;
   for (i = 1; i <= fp->size; i++)
      if (_rm_getbit(fp->rmap, fp->size, i))
	 cnt++;

   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
