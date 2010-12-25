int
clip_RDDSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSKIP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int recs = _clip_parni(ClipMachineMemory, 2);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_skip(ClipMachineMemory, rd, recs, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
