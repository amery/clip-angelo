int
clip_II_OPENTAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_OPENTAG";

   int       h = _clip_parni(ClipMachineMemory, 1);

   const char *tag = _clip_parc(ClipMachineMemory, 2);

   int       er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_opentag(ClipMachineMemory, h, tag, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
