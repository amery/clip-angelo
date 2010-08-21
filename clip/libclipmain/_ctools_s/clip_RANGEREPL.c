int
clip_RANGEREPL(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *e, *end, *r, *ret;

   unsigned char ch, ch1, ch2, ch3;

   unsigned char *s1 = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   int t1 = _clip_parni(ClipMachineMemory, 1);

   unsigned char *s2 = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int t2 = _clip_parni(ClipMachineMemory, 2);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l);

   unsigned char *s3 = (unsigned char *) _clip_parc(ClipMachineMemory, 4);

   int t3 = _clip_parni(ClipMachineMemory, 4);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RANGEREPL");
   }
   ch1 = s1 == NULL ? t1 : *s1;
   ch1 = _clip_cmptbl[ch1];
   ch2 = s2 == NULL ? t2 : *s2;
   ch2 = _clip_cmptbl[ch2];
   ch3 = s3 == NULL ? t3 : *s3;
   ch3 = ch3 == 0 ? ' ' : ch3;
   ch3 = _clip_cmptbl[ch3];
   if (ch1 > ch2)
   {
      ch = ch1;
      ch1 = ch2;
      ch1 = ch;
   }
   ret = malloc(l + 1);
   for (e = str, r = ret, end = str + l; e < end; e++, r++)
   {
      ch = _clip_cmptbl[*e];
      if (ch >= ch1 && ch <= ch2)
	 *r = ch3;
      else
	 *r = *e;
   }
   ret[l] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 2))
      _clip_par_assign_str(ClipMachineMemory, 2, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
