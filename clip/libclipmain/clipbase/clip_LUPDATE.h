int
clip_LUPDATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "LUPDATE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   if (!wa)
      return 0;

   READLOCK;
   if ((er = wa->rd->vtbl->lupdate(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
