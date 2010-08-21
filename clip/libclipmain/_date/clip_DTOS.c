int
clip_DTOS(ClipMachine * ClipMachineMemory)
{
   char *ret;

   long d = _clip_pardj(ClipMachineMemory, 1);

   ret = malloc(9);
   _clip_datetostr(d, ret);
   _clip_retcn_m(ClipMachineMemory, ret, 8);
   return 0;
}
