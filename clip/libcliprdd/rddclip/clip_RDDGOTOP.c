int
clip_RDDGOTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTOP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
