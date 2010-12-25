int
clip_ATAN(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, atan(d), len, dec);
   return 0;
}
