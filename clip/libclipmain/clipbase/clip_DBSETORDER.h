int
clip_DBSETORDER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETORDER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       order = _clip_parni(ClipMachineMemory, 1);

   int       er;


   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, order, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
