int
clip_ATN2(ClipMachine * ClipMachineMemory)
{
   int len1, dec1, len2, dec2;

   double d1 = _clip_parnd(ClipMachineMemory, 1);

   double d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len1, &dec1);
   _clip_parp(ClipMachineMemory, 2, &len2, &dec2);
   dec1 = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, atan2(d1, d2), len1, dec1);
   return 0;
}
