int
rdd_flushbuffer(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, er;

   if (rd->changed)
   {
      if ((er = rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__)))
	 goto err;
      if ((er = rd->vtbl->setrecord(ClipMachineMemory, rd, __PROC__)))
	 goto err_unlock;
      if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
	 goto err;
      rd->changed = 0;
   }
   for (i = 0; i < rd->rels_opened; i++)
   {
      if ((er = rdd_flushbuffer(ClipMachineMemory, rd->relations[i]->child, __PROC__)))
	 goto err;
   }
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
