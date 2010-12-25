int
clip_BEFORATNUM(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2;

   unsigned char *ret, *beg;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int count = _clip_parni(ClipMachineMemory, 3);

   int ignore = _clip_parni(ClipMachineMemory, 4);

   if (sstr == NULL || str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BEFORATNUM");
   }
   beg = _clip_atmupa(ClipMachineMemory, sstr, l1, str, l2, ignore, count, 1);
   l = beg - str;
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
