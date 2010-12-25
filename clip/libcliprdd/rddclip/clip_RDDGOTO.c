int
clip_RDDGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int rec = _clip_parni(ClipMachineMemory, 2);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_goto(ClipMachineMemory, rd, rec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
