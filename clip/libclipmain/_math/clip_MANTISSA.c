int
clip_MANTISSA(ClipMachine * ClipMachineMemory)
{
   int flag_neg = 0, flag = 0;

   double data = _clip_parnd(ClipMachineMemory, 1);

   if (data == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (data < 0)
   {
      flag_neg = 1;
      data = fabs(data);
   }
   if (data < 1)
   {
      flag = 1;
      data = 2 / data;
   }
   for (; data >= 2;)
      data = data / 2;
   if (flag_neg)
      data = 0 - data;
   if (flag)
      data = 2 / data;
   _clip_retndp(ClipMachineMemory, data, 7, 5);
   return 0;
}
