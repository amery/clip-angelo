int
clip_LOG10(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, log10(d), len, dec);
   return 0;
}
