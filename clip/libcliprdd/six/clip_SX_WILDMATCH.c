int
clip_SX_WILDMATCH(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WILDMATCH";

   const char *pattern = _clip_parc(ClipMachineMemory, 1);

   const char *string = _clip_parc(ClipMachineMemory, 2);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, _clip_glob_match(string, pattern, 1) != -1);
   return 0;
 err:
   return er;
}
