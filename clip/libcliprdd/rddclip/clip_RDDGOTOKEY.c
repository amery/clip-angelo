int
clip_RDDGOTOKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTOKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int rec = _clip_parni(ClipMachineMemory, 2);

   int ok, er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   if (rd->curord == -1)
      return 0;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotokey(ClipMachineMemory, rd, rd->orders[rd->curord], rec, &ok, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
