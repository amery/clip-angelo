int
clip_FACT(ClipMachine * ClipMachineMemory)
{
   double ret = 1;

   int i;

   int num = _clip_parni(ClipMachineMemory, 1);

   if (num > 21)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }
   for (i = 1; i <= num; i++)
      ret *= i;
   _clip_retndp(ClipMachineMemory, ret, num, 0);
   return 0;
}
