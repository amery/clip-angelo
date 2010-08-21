int
clip_RDDSETFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *str = _clip_parc(ClipMachineMemory, 2);

   ClipVar *a = _clip_par(ClipMachineMemory, 3);

   int lNoOptimize = _clip_parl(ClipMachineMemory, 4);

   RDD_FILTER *fp;

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, ARRAY_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_clearfilter(ClipMachineMemory, rd, __PROC__)))
      goto err;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_createfilter(ClipMachineMemory, rd, &fp, NULL, str, a, lNoOptimize, __PROC__)))
      goto err_unlock;
   rd->filter = fp;
   fp->active = 1;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
