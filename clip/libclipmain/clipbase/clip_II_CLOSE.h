int
clip_II_CLOSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CLOSE";

   int       h = _clip_parni(ClipMachineMemory, 1);

   int       er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_ii_close(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
