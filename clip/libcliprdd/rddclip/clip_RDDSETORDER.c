int
clip_RDDSETORDER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETORDER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
   {
      int order = _clip_parni(ClipMachineMemory, 2);

      if ((er = rdd_setorder(ClipMachineMemory, rd, order, __PROC__)))
	 goto err_unlock;
   }
   else
   {
      const char *tag = _clip_parc(ClipMachineMemory, 2);

      int i;

      if (tag)
      {
	 for (i = 0; i < rd->ords_opened; i++)
	 {
	    if (strcasecmp(rd->orders[i]->name, tag) == 0)
	    {
	       if ((er = rdd_setorder(ClipMachineMemory, rd, i + 1, __PROC__)))
		  goto err_unlock;
	       break;
	    }
	 }
      }
   }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
