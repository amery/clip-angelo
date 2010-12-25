int
clip_ASCIISUM(ClipMachine * ClipMachineMemory)
{
   int l, i;

   long ret = 0;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
   {
      _clip_retnl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ASCIISUM");
   }
   for (i = 0; i < l; i++)
      ret += str[i];
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
