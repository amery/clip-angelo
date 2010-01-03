int
clip_DBUNLOCKALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUNLOCKALL";

   DBWorkArea *wa;

   int       i, er;


   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
    {
       wa = get_area(ClipMachineMemory, i + 1, 0, 0);
       if (!wa)
	  continue;
       if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
	  goto err;
       READLOCK;
       if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 1, __PROC__)))
	  goto err_unlock;
       UNLOCK;
    }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
