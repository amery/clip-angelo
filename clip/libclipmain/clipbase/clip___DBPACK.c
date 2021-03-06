int
clip___DBPACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBPACK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_pack(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
