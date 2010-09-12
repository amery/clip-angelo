int
clip_DBREAD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBREAD";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   if (!wa)
      return rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_dbread(ClipMachineMemory, wa->rd, RETPTR(ClipMachineMemory), __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
