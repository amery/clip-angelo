int
clip_II_SKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_SKIP";

   int       h = _clip_parni(ClipMachineMemory, 1);

   int       recs = _clip_parni(ClipMachineMemory, 2);

   int       er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = rdd_ii_skip(ClipMachineMemory, h, recs, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
