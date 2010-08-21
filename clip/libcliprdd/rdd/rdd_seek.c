int
rdd_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   RDD_ORDER *ro;

   int er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   ro = rd->orders[rd->curord];
   if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
      return er;
   if ((er = ro->vtbl->seek(ClipMachineMemory, rd, ro, v, soft, last, found, __PROC__)))
      goto unlock;
   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
      return er;
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}
