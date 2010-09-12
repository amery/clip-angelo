int
clip_DBCOMMITALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCOMMITALL";

   DBWorkArea *wa;

   int i, er;

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
   {
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
      if (!wa || !wa->used || wa->rd->readonly)
	 continue;
      WRITELOCK;
      if ((er = rdd_commit(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err_unlock;
      UNLOCK;
   }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
