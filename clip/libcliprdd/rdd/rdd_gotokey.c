int
rdd_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   int er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
      return er;
   if ((er = ro->vtbl->gotokey(ClipMachineMemory, rd, ro, keyno, ok, __PROC__)))
      goto unlock;
   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
      return er;
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}
