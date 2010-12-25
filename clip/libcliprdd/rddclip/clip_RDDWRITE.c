int
clip_RDDWRITE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDWRITE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, MAP_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 1);
   WRITELOCK;
   if ((er = rdd_dbwrite(ClipMachineMemory, rd, ap, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
