int
rdd_skip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int recs, const char *__PROC__)
{
   RDD_ORDER *ro;

   int i, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->curord == -1)
   {
      if (recs > 0)
      {
	 for (i = 0; i < recs; i++)
	 {
	    if ((er = rd->vtbl->next(ClipMachineMemory, rd, 0, __PROC__)))
	       return er;
	 }
      }
      else
      {
	 for (i = 0; i > recs; i--)
	 {
	    if ((er = rd->vtbl->prev(ClipMachineMemory, rd, __PROC__)))
	       return er;
	 }
      }
   }
   else
   {
      ro = rd->orders[rd->curord];
      if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
	 return er;
      if (recs > 0)
      {
	 for (i = 0; i < recs; i++)
	 {
	    if ((er = ro->vtbl->next(ClipMachineMemory, rd, ro, __PROC__)))
	       goto unlock;
	 }
      }
      else
      {
	 for (i = 0; i > recs; i--)
	 {
	    if ((er = ro->vtbl->prev(ClipMachineMemory, rd, ro, __PROC__)))
	       goto unlock;
	 }
      }
      if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	 return er;
   }
   if (recs == 0)
   {
      rd->valid = 0;
   }
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}
