int
clip_RLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int r, er;

   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (!(ClipMachineMemory->flags1 & MULTILOCKS_FLAG))
   {
      if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 0, __PROC__)))
	 goto err_unlock;
   }
   if (wa->rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err_unlock;
   if ((er = rdd_rlock(ClipMachineMemory, wa->rd, wa->rd->recno, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
