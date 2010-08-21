int
clip_INT(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = 0;
#ifdef ARCH_i386
   (*(long long *) &d)++;
#endif
   _clip_retndp(ClipMachineMemory, d - fmod(d, 1.00), len, dec);
   return 0;
}
