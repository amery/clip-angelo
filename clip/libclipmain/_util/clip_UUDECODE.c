int
clip_UUDECODE(ClipMachine * ClipMachineMemory)
{
   char *str, *out = 0;

   long ol;

   int l;

   str = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!str)
      return EG_ARG;

   if (_clip_uudecode(str, l, &out, &ol))
      return EG_ARG;
   _clip_retcn_m(ClipMachineMemory, out, ol);

   return 0;
}
