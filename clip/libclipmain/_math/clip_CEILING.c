int
clip_CEILING(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = 0;
   _clip_retndp(ClipMachineMemory, ceil(d), len, dec);
   return 0;
}
