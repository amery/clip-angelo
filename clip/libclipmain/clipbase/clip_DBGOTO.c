int
clip_DBGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBGOTO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int rec = _clip_parni(ClipMachineMemory, 1);

   int er;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_goto(ClipMachineMemory, wa->rd, rec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
