int
clip_RLOCKVERIFY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RLOCKVERIFY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int r, er;

   CHECKWA(wa);

   READLOCK;
   if ((er = wa->rd->vtbl->verify(ClipMachineMemory, wa->rd, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if (r)
   {
      return clip_RLOCK(ClipMachineMemory);
   }
   _clip_retl(ClipMachineMemory, 0);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
