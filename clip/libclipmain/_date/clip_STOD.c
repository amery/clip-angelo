int
clip_STOD(ClipMachine * ClipMachineMemory)
{
   long d, m, y;

   char *str = _clip_parc(ClipMachineMemory, 1);

   if (str == NULL)
   {
      _clip_retdj(ClipMachineMemory, 0);
      return 0;
      /*
         return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STOD");
       */
   }
   if (str[0] == ' ' || str[0] == '\t')
   {
      _clip_retdj(ClipMachineMemory, 0);
      return 0;
   }
   y = atol(str) / 10000;
   m = atol(str + 4) / 100;
   d = atol(str + 6);
   _clip_retdj(ClipMachineMemory, _clip_jdate(d, m, y));
   return 0;
}
