int
clip_DBUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUNLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      return er;
   READLOCK;
   if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
