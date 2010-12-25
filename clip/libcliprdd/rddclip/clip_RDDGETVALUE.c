int
clip_RDDGETVALUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGETVALUE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int type = _clip_parinfo(ClipMachineMemory, 2);

   int er;

   if (!rd)
      return EG_NOTABLE;
   READLOCK;
   if (type == CHARACTER_type_of_ClipVarType)
   {
      const char *fname = _clip_parc(ClipMachineMemory, 2);

      if ((er = rdd_getvaluebn(ClipMachineMemory, rd, fname, __PROC__)))
	 goto err_unlock;
   }
   else if (type == NUMERIC_type_of_ClipVarType)
   {
      int no = _clip_parni(ClipMachineMemory, 2) - 1;

      if ((er = rdd_getvalue(ClipMachineMemory, rd, no, __PROC__)))
	 goto err_unlock;
   }
   else
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);
      goto err_unlock;
   }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
