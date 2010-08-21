int
clip_RDDSETINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   const char *tag = _clip_parc(ClipMachineMemory, 4);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if ((er = rdd_setindex(ClipMachineMemory, rd, NULL, driver, name, tag, 0, __PROC__)))
      goto err_unlock;
   if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
