int
clip_RDDLASTKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLASTKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int lastkey = 0, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_lastkey(ClipMachineMemory, rd, &lastkey, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastkey);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
