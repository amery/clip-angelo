int
clip_RDDDELETED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDDELETED";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int deleted, er;

   if (!rd)
      return EG_NOTABLE;

   READLOCK;
   if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, deleted);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
