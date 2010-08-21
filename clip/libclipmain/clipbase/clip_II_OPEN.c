int
clip_II_OPEN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_OPEN";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   int er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if (driver && driver[0])
   {
      if ((er = rdd_ii_open(ClipMachineMemory, driver, name, __PROC__)))
	 goto err;
   }
   else
   {
      char idx[4];

      if ((er = db_driver(ClipMachineMemory, NULL, NULL, idx, NULL, __PROC__)))
	 goto err;
      if ((er = rdd_ii_open(ClipMachineMemory, idx, name, __PROC__)))
	 goto err;
   }
   return 0;
 err:
   return er;
}
