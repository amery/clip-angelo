int
rdd_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_ORDER *ro;

   int er;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->curord == -1)
   {
      if ((er = rd->vtbl->gotop(ClipMachineMemory, rd, __PROC__)))
	 return er;
   }
   else
   {
      ro = rd->orders[rd->curord];
      if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
	 return er;
      if ((er = ro->vtbl->gotop(ClipMachineMemory, rd, ro, __PROC__)))
	 goto unlock;
      if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	 return er;
   }
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}
