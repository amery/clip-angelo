int
clip_ABS(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   _clip_retndp(ClipMachineMemory, fabs(d), len, dec);
   return 0;
}
