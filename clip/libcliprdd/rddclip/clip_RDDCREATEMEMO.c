int
clip_RDDCREATEMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATEMEMO";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   int er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_creatememo(ClipMachineMemory, driver, name, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
