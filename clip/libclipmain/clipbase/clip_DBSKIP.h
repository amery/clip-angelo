int
clip_DBSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSKIP";

   int       recs = _clip_parni(ClipMachineMemory, 1);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_skip(ClipMachineMemory, wa->rd, recs, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
