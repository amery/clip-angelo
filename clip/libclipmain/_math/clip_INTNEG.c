int
clip_INTNEG(ClipMachine * ClipMachineMemory)
{
   double data = _clip_parnd(ClipMachineMemory, 1);

   char *str = _clip_parc(ClipMachineMemory, 1);

   int flag = _clip_parl(ClipMachineMemory, 2);

   if (str != NULL)
      data = (double) _clip_strtod_base(str, 16);
   if (data <= 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (flag)
      _clip_retnl(ClipMachineMemory, (unsigned long) data);
   else
      _clip_retnl(ClipMachineMemory, (short) data);
   return 0;
}
