int
clip_RDDSETRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETRELATION";

   int parent = _clip_parni(ClipMachineMemory, 1);

   int child = _clip_parni(ClipMachineMemory, 2);

   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, parent, _C_ITEM_TYPE_RDD);

   RDD_DATA *crd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, child, _C_ITEM_TYPE_RDD);

   const char *expr = _clip_parc(ClipMachineMemory, 3);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      crd = NULL;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      crd = NULL;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setrelation(ClipMachineMemory, rd, crd, NULL, expr, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
