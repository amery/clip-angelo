int
clip_BLANK(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *str, *ret;

   int t = _clip_parinfo(ClipMachineMemory, 1);

   int flag = _clip_parl(ClipMachineMemory, 2);

   if (t == UNDEF_type_of_ClipVarType || t == LOGICAL_type_of_ClipVarType)
      _clip_retl(ClipMachineMemory, 0);

   if (t == NUMERIC_type_of_ClipVarType)
      _clip_retndp(ClipMachineMemory, 0, 10, 0);

   if (t == DATE_type_of_ClipVarType)
      _clip_retdj(ClipMachineMemory, 0);

   if (t == CHARACTER_type_of_ClipVarType && !flag)
      _clip_retc(ClipMachineMemory, "");

   if (t == CHARACTER_type_of_ClipVarType && flag)
   {
      str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);
      ret = malloc(l + 1);
      memset(ret, ' ', l);
      ret[l] = 0;
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   }
   return 0;
}
