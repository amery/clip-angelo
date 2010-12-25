int
clip_INVERTATTR(ClipMachine * ClipMachineMemory)
{
   int l, t, attr = _clip_parni(ClipMachineMemory, 1);

   char *s = _clip_parcl(ClipMachineMemory, 1, &l);

   _clip_retni(ClipMachineMemory, 0);
   t = _clip_parinfo(ClipMachineMemory, 1);

   if (t != NUMERIC_type_of_ClipVarType && t != CHARACTER_type_of_ClipVarType)
      return 0;
   if (t == CHARACTER_type_of_ClipVarType)
      attr = _clip_str2attr(s, l);
   attr = ((attr & 0x88) + ((attr & 0x07) << 4) + ((attr & 0x70) >> 4));
   _clip_retni(ClipMachineMemory, attr);
   return 0;
}
