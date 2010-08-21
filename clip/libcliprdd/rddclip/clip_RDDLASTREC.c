int
clip_RDDLASTREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLASTREC";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int lastrec, er;

   if (!rd)
      return EG_NOTABLE;

   READLOCK;
   if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastrec);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
