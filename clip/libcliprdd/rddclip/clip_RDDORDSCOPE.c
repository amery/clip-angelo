int
clip_RDDORDSCOPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDORDSCOPE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int scope = _clip_parni(ClipMachineMemory, 2);

   ClipVar *v = _clip_par(ClipMachineMemory, 3);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if (scope == 0)
   {
      if ((er = rdd_scopetop(ClipMachineMemory, rd, v, __PROC__)))
	 goto err_unlock;
   }
   else if (scope == 1)
   {
      if ((er = rdd_scopebottom(ClipMachineMemory, rd, v, __PROC__)))
	 goto err_unlock;
   }
   else
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badargument2);
      goto err_unlock;
   }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
