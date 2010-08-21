int
clip_RDDUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDUNLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int recno = _clip_parni(ClipMachineMemory, 2);

   int er;

   if (!rd)
      return EG_NOTABLE;

   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
   {
      recno = rd->recno;
   }

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if ((er = rdd_ulock(ClipMachineMemory, rd, recno, 1, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
