int
clip_CHAREVEN(ClipMachine * ClipMachineMemory)
{
   int l, l1, i;

   unsigned char *ret, *e, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHAREVEN");
   }
   l = l1 / 2;
   ret = malloc(l + 1);
   for (i = 0, e = str + 1, end = str + l1; e < end; e += 2, i++)
      ret[i] = *e;
   ret[l] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
