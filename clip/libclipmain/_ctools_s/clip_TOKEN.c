int
clip_TOKEN(ClipMachine * ClipMachineMemory)
{
   int l1, l2, count = 1, nt, *tmp1;

   unsigned char *e, *end, *dstr, *buf, *ret, *tbeg, *tend, *tmp2, tsep;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int num = _clip_parni(ClipMachineMemory, 3);

   int tlen = _clip_parni(ClipMachineMemory, 4);

   int ignore = _clip_parni(ClipMachineMemory, 5);

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   tmp2 = atsep + 3 * sizeof(int);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TOKEN");
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
   {
      num = _clip_parni(ClipMachineMemory, 2);
      sstr = NULL;
   }
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
      sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l2);

   if (sstr == NULL)
   {
      dstr = (unsigned char *) CLIP_TOKEN_CHARACTERS;
      l2 = CLIP_TOKEN_CHARS_NUM;
   }
   else
      dstr = sstr;
   if (tlen <= 0)
      tlen = CLIP_TOKEN_MAXLEN;
   buf = calloc(256, 1);
   for (e = dstr; e < dstr + l2; e++)
      buf[(int) (*e)] = 1;
   end = str + l1;
   for (nt = 0; nt < tlen && end > str + ignore && buf[(int) (*end)]; end--, nt++);
   if (*end != 0)
      end++;
   tend = end;
   tbeg = num <= 2 ? str + ignore : end;
   *tmp2 = 0;
   *(tmp2 + 2) = *tend;
   for (e = str + ignore; e < end;)
   {
      for (; e < end && !buf[(int) (*e)]; e++);
      if (e == end)
	 break;
      tsep = *e;
      if (count == num && num != 0)
      {
	 *(tmp2 + 2) = *(e);
	 tend = e;
      }
      for (nt = 0; nt < tlen && e < end && buf[(int) (*e)]; e++, nt++);
      count++;
      if (count == num || num == 0)
      {
	 *tmp2 = tsep;
	 tbeg = e;
      }
   }
   if (count < num)
   {
      free(buf);
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   tbeg = tbeg > tend ? tend : tbeg;
   ret = malloc(tend - tbeg + 1);
   memcpy(ret, tbeg, tend - tbeg);
   ret[tend - tbeg] = 0;
   /* set data for tokenat,tokensep */
   tmp1 = (int *) atsep;
   tmp1++;
   *tmp1 = tbeg - str + 1;
   tmp1++;
   *tmp1 = tend - str + 1;

   _clip_retcn_m(ClipMachineMemory, (char *) ret, tend - tbeg);
   free(buf);
   return 0;
}
