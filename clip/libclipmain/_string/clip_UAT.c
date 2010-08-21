int
clip_UAT(ClipMachine * ClipMachineMemory)
{
   int sl, l, ll;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int from = _clip_parni(ClipMachineMemory, 3);

   int to = _clip_parni(ClipMachineMemory, 4);

   int direct = _clip_parni(ClipMachineMemory, 5);

   const char *ss = NULL;

   if (str == NULL || s == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RAT");
   }

   if (from <= 0)
      from = 1;
   from--;

   if (to > l || to <= 0)
      to = l;
   to--;

   if (direct >= 0)
      ss = _atl(str + from, s, to - from + 1, sl);
   else
      ss = _atr(str + from, s, to - from + 1, sl);

   ll = ss - str;
   if (ss == NULL || (ll + sl) > (to + 1))
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, ss - str + 1);

   return 0;
}
