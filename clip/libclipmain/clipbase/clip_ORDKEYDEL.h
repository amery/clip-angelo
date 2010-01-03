int
clip_ORDKEYDEL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYDEL";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *order = _clip_par(ClipMachineMemory, 1);

   ClipVar  *index = _clip_par(ClipMachineMemory, 2);

   int       ord, er;


   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_keydel(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
