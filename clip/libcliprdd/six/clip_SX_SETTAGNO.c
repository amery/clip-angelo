int
clip_SX_SETTAGNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETTAGNO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   ClipVar in;

   int ord, er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      return 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);

   if (_clip_parinfo(ClipMachineMemory, 2) == 0)
   {
      index = &in;
      if (wa->rd->curord == -1)
	 return 0;
      memset(index, 0, sizeof(ClipVar));
      index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = wa->rd->orders[wa->rd->curord]->index->indexno;
   }

   ord = get_orderno(wa, order, index);
   if (ord < -1 || ord >= wa->rd->ords_opened || _clip_parinfo(ClipMachineMemory, 0) == 0)
      return 0;

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ord + 1, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
