int
rdd_checkifnew(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_ORDER *ro;

   int i, er = 0;

   _rdd_freebuf(ClipMachineMemory, rd);
   if (rd->newrec)
   {
      if (rd->ords_opened > 0)
      {
	 for (i = 0; i < rd->ords_opened; i++)
	 {
	    ro = rd->orders[i];
	    if (!ro->custom && rd->orders[i]->canadd)
	    {
	       if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		  return er;
	       if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    }
	 }
      }
      rd->newrec = 0;
   }
   return 0;
 unlock:
   for (i = 0; i < rd->ords_opened; i++)
   {
      ro = rd->orders[i];
      if (!ro->custom && rd->orders[i]->canadd)
      {
	 ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
      }
   }
   return er;
}
