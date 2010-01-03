int
clip___DBCONTINUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBCONTINUE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       found, er;


   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_continue(ClipMachineMemory, wa->rd, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
