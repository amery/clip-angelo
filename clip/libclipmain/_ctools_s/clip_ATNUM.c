int
clip_ATNUM(ClipMachine * ClipMachineMemory)
{
   int l1, l2;

   unsigned char *beg;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int count = _clip_parni(ClipMachineMemory, 3);

   int ignore = _clip_parni(ClipMachineMemory, 4);

   if (sstr == NULL || str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ATNUM");
   }
   beg = _clip_atmupa(ClipMachineMemory, sstr, l1, str, l2, ignore, count, 1);
   if (beg < str + l2)
      _clip_retni(ClipMachineMemory, beg - str + 1);
   else
      _clip_retni(ClipMachineMemory, 0);
   return 0;
}
