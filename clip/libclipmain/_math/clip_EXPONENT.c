int
clip_EXPONENT(ClipMachine * ClipMachineMemory)
{
   int ret = 0, flag = 0;

   double data = fabs(_clip_parnd(ClipMachineMemory, 1));

   if (data == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (data < 1)
   {
      flag = 1;
      data = 2 / data;
   }
   for (; data >= 2; ret++)
      data = data / 2;
   if (flag)
      ret = 0 - ret;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
