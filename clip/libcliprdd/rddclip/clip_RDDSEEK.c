int
clip_RDDSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSEEK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar *v = _clip_par(ClipMachineMemory, 2);

   int soft = _clip_parl(ClipMachineMemory, 3);

   int last = _clip_parl(ClipMachineMemory, 4);

   int found, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (!v)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      soft = (ClipMachineMemory->flags & SOFTSEEK_FLAG);
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_seek(ClipMachineMemory, rd, v, soft, last, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
