/*
   str2var(cStrRepresentation) -> var
 */
int
clip_STR2VAR(ClipMachine * ClipMachineMemory)
{
   int len;

   char *str = _clip_parcl(ClipMachineMemory, 1, &len);

   if (!str)
      return EG_ARG;

   _clip_str2var(ClipMachineMemory, RETPTR(ClipMachineMemory), str, len, 1);

   return 0;
}
