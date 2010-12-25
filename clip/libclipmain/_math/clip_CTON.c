int
clip_CTON(ClipMachine * ClipMachineMemory)
{
   long ret = 0;

   char *str = _clip_parc(ClipMachineMemory, 1);

   int base = _clip_parni(ClipMachineMemory, 2);

   if (base <= 0 || base > 36)
      base = 10;
   if (str != NULL)
      ret = strtol(str, NULL, base);
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
