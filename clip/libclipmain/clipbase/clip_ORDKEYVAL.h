int
clip_ORDKEYVAL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYVAL";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   CHECKWA(wa);

   READLOCK;
   if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, RETPTR(ClipMachineMemory), __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
