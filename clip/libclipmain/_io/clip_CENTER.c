int
clip_CENTER(ClipMachine * ClipMachineMemory)
{
   int l1, l2, nl, i;

   unsigned char *ret, *beg, *end, fillchr = ' ';

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int len = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l2);

   int flag = _clip_parl(ClipMachineMemory, 4);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CENTER");
   }
   if (s != NULL)
      fillchr = *s;
   if (_clip_parinfo(ClipMachineMemory, 3) == LOGICAL_type_of_ClipVarType)
   {
      flag = _clip_parl(ClipMachineMemory, 3);
      s = (unsigned char *) (" ");
   }
   if (len == 0)
   {
      len = 80;
      _clip_fullscreen(ClipMachineMemory);
      if (ClipMachineMemory->fullscreen)
      {
	 ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	 len = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
      }
   }
   ret = malloc(len + 1);
   for (beg = str; beg < str + l1 && (*beg == ' ' || *beg == '\t'); beg++);
   for (end = str + l1 - 1; end > beg && (*end == ' ' || *end == '\t'); end--);
   end++;
   nl = (len - (end - beg)) / 2;
   for (i = 0; i < nl; i++)
      ret[i] = fillchr;
   for (; beg < end; beg++, i++)
      ret[i] = *beg;
   if (flag)
   {
      for (; i < len; i++)
	 ret[i] = fillchr;
   }
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i);
   return 0;
}
