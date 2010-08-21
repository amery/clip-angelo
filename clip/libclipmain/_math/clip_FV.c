int
clip_FV(ClipMachine * ClipMachineMemory)
{
   int len, dec, i;

   double ret;

   double sum = _clip_parnd(ClipMachineMemory, 1);

   double prec = 1.00 + _clip_parnd(ClipMachineMemory, 2);

   int per = _clip_parni(ClipMachineMemory, 3);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   for (ret = sum, i = 0; i < per; i++)
      ret *= prec;
   _clip_retndp(ClipMachineMemory, ret, len, dec);
   return 0;
}
