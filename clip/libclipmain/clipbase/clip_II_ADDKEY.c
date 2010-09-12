int
clip_II_ADDKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_ADDKEY";

   int h = _clip_parni(ClipMachineMemory, 1);

   const char *id = _clip_parc(ClipMachineMemory, 2);

   ClipVar *key = _clip_par(ClipMachineMemory, 3);

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_addkey(ClipMachineMemory, h, id, key, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
