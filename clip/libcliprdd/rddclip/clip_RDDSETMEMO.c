int
clip_RDDSETMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETMEMO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_setmemo(ClipMachineMemory, rd, driver, name, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
