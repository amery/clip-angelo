int
clip_CHARSPREAD(ClipMachine * ClipMachineMemory)
{
   int i, l1, l2, nl = 0, sl, cpos = 0, nch, och;

   unsigned char *e, *end, *ret, *cur;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int l = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l2);

   int ch = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL || l == 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARSPREAD");
   }
   if (s != NULL)
      ch = *s;
   if (ch == 0)
      ch = ' ';
   ret = malloc(l + 1);
   memcpy(ret, str, l1);
   ret[l1] = 0;
   for (e = str, end = str + l1; e < end;)
   {
      for (; *e != ch && e < end; e++);
      if (*e != ch)
	 continue;
      nl++;
      for (; *e == ch && e < end; e++);
   }
   if (nl == 0)
   {
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l1);
      return 0;
   }
   sl = l - l1;
   nch = sl / nl;
   och = sl % nl;
   cur = str;
   for (e = str, cur = str, end = str + l1; e < end;)
   {
      for (; *e != ch && e < end; e++);
      if (*e != ch)
	 break;
      memcpy(ret + cpos, cur, e - cur);
      cpos += e - cur;
      for (i = 0; i < nch; i++, cpos++)
	 ret[cpos] = ch;
      if (och > 0)
      {
	 ret[cpos] = ch;
	 cpos++;
	 och--;
      }
      cur = e;
      for (; *e == ch && e < end; e++);
   }
   memcpy(ret + cpos, cur, e - cur);
   ret[l] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
