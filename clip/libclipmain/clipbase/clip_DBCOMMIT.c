int
clip_DBCOMMIT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCOMMIT";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   CHECKWA(wa);

   WRITELOCK;
   if ((er = rdd_commit(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
