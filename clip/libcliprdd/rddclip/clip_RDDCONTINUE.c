int
clip_RDDCONTINUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCONTINUE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int found;

   int er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_continue(ClipMachineMemory, rd, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
