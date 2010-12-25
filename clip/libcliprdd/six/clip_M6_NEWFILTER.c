int
clip_M6_NEWFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_NEWFILTER";

   int type = _clip_parinfo(ClipMachineMemory, 1);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   RDD_FILTER *fp;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (!wa)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if (type == NUMERIC_type_of_ClipVarType || type == UNDEF_type_of_ClipVarType)
   {
      unsigned int len = _clip_parni(ClipMachineMemory, 1);

      if ((er = rdd_createuserfilter(ClipMachineMemory, wa->rd, &fp, len, __PROC__)))
	 goto err_unlock;
   }
   else if (type == CHARACTER_type_of_ClipVarType)
   {
      char *str = _clip_parc(ClipMachineMemory, 1);

      if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, NULL, str, NULL, 0, __PROC__)))
	 goto err_unlock;
   }
   UNLOCK;

   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
