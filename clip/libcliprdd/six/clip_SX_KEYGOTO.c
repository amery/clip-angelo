int
clip_SX_KEYGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KEYGOTO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   unsigned int keyno = _clip_parni(ClipMachineMemory, 3);

   int ord, ok, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   _clip_retl(ClipMachineMemory, 0);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if (ord == -1)
   {
      if ((er = rdd_goto(ClipMachineMemory, wa->rd, keyno, __PROC__)))
	 goto err_unlock;
      ok = 1;
   }
   else
   {
      if ((er = rdd_gotokey(ClipMachineMemory, wa->rd, wa->rd->orders[ord], keyno, &ok, __PROC__)))
	 goto err_unlock;
   }
   UNLOCK;

   _clip_retl(ClipMachineMemory, ok);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
