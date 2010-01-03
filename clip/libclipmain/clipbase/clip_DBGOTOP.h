int
clip_DBGOTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBGOTOP";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
