int
clip_POSALPHA(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *e, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int flag = _clip_parl(ClipMachineMemory, 2);

   int ignore = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSALPHA");
   }
   if (ignore < 0)
      ignore = 0;
   for (e = str + ignore, end = str + l; e < end && !(_clip_isalpha(*e) ^ flag); e++);
   if (e == end)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, e - str + 1);
   return 0;
}
