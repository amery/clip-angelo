int
clip_FLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   if (!wa)
      return 0;

   READLOCK;
   if ((er = rdd_flock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
