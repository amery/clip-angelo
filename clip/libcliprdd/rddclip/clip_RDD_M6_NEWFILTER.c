int
clip_RDD_M6_NEWFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_NEWFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int type = _clip_parinfo(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if (type == NUMERIC_type_of_ClipVarType || type == UNDEF_type_of_ClipVarType)
   {
      unsigned int len = _clip_parni(ClipMachineMemory, 2);

      if ((er = rdd_createuserfilter(ClipMachineMemory, rd, &fp, len, __PROC__)))
	 goto err_unlock;
   }
   else if (type == CHARACTER_type_of_ClipVarType)
   {
      char *str = _clip_parc(ClipMachineMemory, 2);

      if ((er = rdd_createfilter(ClipMachineMemory, rd, &fp, NULL, str, NULL, 0, __PROC__)))
	 goto err_unlock;
   }
   UNLOCK;

   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;

 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
