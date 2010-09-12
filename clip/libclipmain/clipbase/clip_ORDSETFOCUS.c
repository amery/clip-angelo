int
clip_ORDSETFOCUS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSETFOCUS";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int ord, er;

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (wa->rd->curord != -1)
      _clip_retc(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->name);

   if (!_clip_parinfo(ClipMachineMemory, 0))
      return 0;

   if ((order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType) && (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0))
   {
      ord = -1;
   }
   else
   {
      ord = get_orderno(wa, order, index);
      if (ord < 0 || ord >= wa->rd->ords_opened || _clip_parinfo(ClipMachineMemory, 0) == 0)
	 return 0;
   }

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ord + 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
