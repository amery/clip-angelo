int
clip_POW(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   double d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, pow(d, d2), len, dec);
   return 0;
}
