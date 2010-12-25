int
clip_SPACE(ClipMachine * ClipMachineMemory)
{
   int kol = _clip_parni(ClipMachineMemory, 1);

   char *ret;

   if (kol <= 0)
      kol = 0;
   ret = malloc(kol + 1);
   if (!_clip_parl(ClipMachineMemory, 2))
      memset(ret, ' ', kol);
   ret[kol] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, kol);
   return 0;
}
