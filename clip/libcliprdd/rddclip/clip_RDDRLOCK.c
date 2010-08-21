int
clip_RDDRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDRLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int recno = _clip_parni(ClipMachineMemory, 2);

   int lrelease = _clip_parl(ClipMachineMemory, 3);

   int r, er;

   if (!rd)
      return EG_NOTABLE;

   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if (recno == 0)
      recno = rd->recno;

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      lrelease = 1;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (lrelease)
   {
      if ((er = rdd_ulock(ClipMachineMemory, rd, 0, 0, __PROC__)))
	 goto err;
   }

   if ((er = rdd_rlock(ClipMachineMemory, rd, recno, &r, __PROC__)))
      goto err;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}
