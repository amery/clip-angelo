int
clip_BT_FIELDFILL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_FIELDFILL";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   u4 oldrecno;

   int oldbof, oldeof, fno, er;

   ClipVar v;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG2(2, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKBTREE(bt);

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
   {
      fno = _clip_parni(ClipMachineMemory, 2) - 1;
   }
   else
   {
      int l;

      const char *fname = _clip_parcl(ClipMachineMemory, 2, &l);

      fno = _rdd_fieldno(wa->rd, _clip_casehashword(fname, l));
   }

   if (fno < 0 || fno >= wa->rd->nfields)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);
      goto err;
   }
   oldrecno = wa->rd->recno;
   oldbof = wa->rd->bof;
   oldeof = wa->rd->eof;
   if ((er = wa->rd->vtbl->gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   while (!wa->rd->eof)
   {
      if ((er = wa->rd->vtbl->next(ClipMachineMemory, wa->rd, 1, __PROC__)))
	 goto err;
      if (wa->rd->eof)
	 break;
      if ((er = wa->rd->vtbl->getvalue(ClipMachineMemory, wa->rd, fno, &v, __PROC__)))
	 goto err;
      if (bt->type == 'I')
      {
	 u4 n = (u4) v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

	 if (bt_add(bt, NULL, &n))
	 {
	    er = rdd_err(ClipMachineMemory, -1, 0, __FILE__, __LINE__, __PROC__, er_internal);
	    goto err;
	 }
      }
   }
   wa->rd->recno = oldrecno;
   wa->rd->bof = oldbof;
   wa->rd->eof = oldeof;
   if ((er = rdd_childs(ClipMachineMemory, wa->rd, __PROC__)))
      return er;
   return 0;
 err:
   return er;
}
