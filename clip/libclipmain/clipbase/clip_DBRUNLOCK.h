int
clip_DBRUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRUNLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   unsigned int rec = _clip_parni(ClipMachineMemory, 1);

   int       er;


   if (!wa)
      return 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_ulock(ClipMachineMemory, wa->rd, rec, 0, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
