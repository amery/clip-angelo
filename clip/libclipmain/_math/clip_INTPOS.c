int
clip_INTPOS(ClipMachine * ClipMachineMemory)
{
   double data = _clip_parnd(ClipMachineMemory, 1);

   char *str = _clip_parc(ClipMachineMemory, 1);

   int flag = _clip_parl(ClipMachineMemory, 2);

   if (str != NULL)
      data = (double) _clip_strtod_base(str, 16);
   if (flag)
      _clip_retnd(ClipMachineMemory, (unsigned long) data);
   else
      _clip_retnl(ClipMachineMemory, (unsigned short) data);
   return 0;
}
