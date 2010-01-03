int
clip_DELETED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DELETED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       deleted, er;

   int       locked = wa->rd->file.rlocked || wa->rd->file.wlocked;


   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (!locked)
    {
       READLOCK;
    }
   if ((er = rdd_deleted(ClipMachineMemory, wa->rd, &deleted, __PROC__)))
      goto err_unlock;
   if (!locked)
    {
       UNLOCK;
    }
   _clip_retl(ClipMachineMemory, deleted);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
