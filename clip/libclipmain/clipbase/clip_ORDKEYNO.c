int
clip_ORDKEYNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYNO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int ord = _clip_parni(ClipMachineMemory, 1);

   int r, er;

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType && !ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);
   if (ord == -1)
   {
      _clip_retni(ClipMachineMemory, wa->rd->recno);
      return 0;
   }

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_checkifnew(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   if ((er = wa->rd->orders[ord]->vtbl->keyno(ClipMachineMemory, wa->rd, wa->rd->orders[ord], &r, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retni(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
