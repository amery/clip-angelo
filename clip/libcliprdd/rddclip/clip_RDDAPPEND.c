int
clip_RDDAPPEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDAPPEND";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   int neterr, er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, MAP_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_append(ClipMachineMemory, rd, &neterr, __PROC__)))
      goto err_unlock;
   if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      if ((er = rdd_dbwrite(ClipMachineMemory, rd, ap, __PROC__)))
	 goto err_unlock;
   UNLOCK;
   if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
	 goto err;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
