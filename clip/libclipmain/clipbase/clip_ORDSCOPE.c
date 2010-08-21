int
clip_ORDSCOPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSCOPE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int scope = _clip_parni(ClipMachineMemory, 1);

   ClipVar *v = _clip_par(ClipMachineMemory, 2);

   int er;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (scope == 0)
   {
      if ((er = rdd_scopetop(ClipMachineMemory, wa->rd, v, __PROC__)))
	 goto err_unlock;
   }
   else if (scope == 1)
   {
      if ((er = rdd_scopebottom(ClipMachineMemory, wa->rd, v, __PROC__)))
	 goto err_unlock;
   }
   else
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Bad argument (1)");
      goto err_unlock;
   }
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
