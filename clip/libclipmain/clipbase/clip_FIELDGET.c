int
clip_FIELDGET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDGET";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int fno = _clip_parni(ClipMachineMemory, 1);

   int er;

   if (!wa)
      return 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 1 || fno > wa->rd->nfields)
      return 0;

   READLOCK;
   if ((er = rdd_getvalue(ClipMachineMemory, wa->rd, fno - 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
