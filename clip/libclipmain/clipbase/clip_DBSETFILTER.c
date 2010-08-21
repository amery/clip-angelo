int
clip_DBSETFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETFILTER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *block = _clip_spar(ClipMachineMemory, 1);

   const char *str = _clip_parc(ClipMachineMemory, 2);

   int lNoOptimize = _clip_parl(ClipMachineMemory, 3);

   RDD_FILTER *fp;

   int er;

   CHECKWA(wa);

   CHECKOPT2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_clearfilter(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

   if (_clip_parinfo(ClipMachineMemory, 1) != CCODE_type_of_ClipVarType && _clip_parinfo(ClipMachineMemory, 1) != PCODE_type_of_ClipVarType)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, block, str, NULL, lNoOptimize, __PROC__)))
      goto err_unlock;
   wa->rd->filter = fp;
   fp->active = 1;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, wa->rd, fp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
