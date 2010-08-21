int
clip_SIGN(ClipMachine * ClipMachineMemory)
{
   int len, dec, ret = 0;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (d > 0)
      ret = 1;
   if (d < 0)
      ret = -1;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
