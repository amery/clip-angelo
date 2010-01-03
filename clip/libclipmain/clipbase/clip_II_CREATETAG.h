int
clip_II_CREATETAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CREATETAG";

   int       h = _clip_parni(ClipMachineMemory, 1);

   const char *tag = _clip_parc(ClipMachineMemory, 2);

   const char *expr = _clip_parc(ClipMachineMemory, 3);

   int       er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_createtag(ClipMachineMemory, h, tag, expr, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
