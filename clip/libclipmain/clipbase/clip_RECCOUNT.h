int
clip_RECCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RECCOUNT";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       lastrec, er;


   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   READLOCK;
   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, &lastrec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastrec);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
