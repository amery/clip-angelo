int
clip_POSRANGE(ClipMachine * ClipMachineMemory)
{
   int l, x;

   unsigned char *e, *end;

   unsigned char ch, ch1, ch2;

   unsigned char *s1 = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   unsigned char *s2 = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int t1 = _clip_parni(ClipMachineMemory, 1);

   int t2 = _clip_parni(ClipMachineMemory, 2);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l);

   int flag = _clip_parl(ClipMachineMemory, 4);

   int ignore = _clip_parni(ClipMachineMemory, 5);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSRANGE");
   }
   if (ignore < 0)
      ignore = 0;
   ch1 = s1 == NULL ? t1 : *s1;
   ch2 = s2 == NULL ? t2 : *s2;
   ch1 = _clip_cmptbl[ch1];
   ch2 = _clip_cmptbl[ch2];
   if (ch1 > ch2)
   {
      ch = ch1;
      ch1 = ch2;
      ch1 = ch;
   }
   for (e = str + ignore, end = str + l; e < end; e++)
   {
      ch = _clip_cmptbl[*e];
      x = (ch >= ch1 && ch <= ch2);
      if (x != flag)
	 break;
   }
   if (e == end)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, e - str + 1);
   return 0;
}
