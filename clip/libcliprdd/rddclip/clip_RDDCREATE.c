int
clip_RDDCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATE";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   ClipVar *stru = _clip_par(ClipMachineMemory, 3);

   int er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, ARRAY_type_of_ClipVarType);

   if ((er = rdd_create(ClipMachineMemory, driver, NULL, name, stru, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
