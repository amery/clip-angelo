int
_clip_token_case(ClipMachine * ClipMachineMemory, int flag)
{
   int l1, l2, count = 0;

   unsigned char *e, *end, *dstr, *buf, *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int num = _clip_parni(ClipMachineMemory, 3);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CLIP_TOKEN");
   }

   if (sstr == NULL)
   {
      dstr = (unsigned char *) CLIP_TOKEN_CHARACTERS;
      l2 = CLIP_TOKEN_CHARS_NUM;
   }
   else
      dstr = sstr;

   buf = calloc(256, 1);
   for (e = dstr; e < dstr + l2; e++)
      buf[(int) (*e)] = 1;
   ret = malloc(l1 + 1);
   memcpy(ret, str, l1);
   ret[l1] = 0;
   end = ret + l1;

   for (; end > ret && buf[(int) (*end)]; end--);
   end++;
   for (e = ret; e < end;)
   {
      for (; e < end && !buf[(int) (*e)]; e++);
      for (; e < end && buf[(int) (*e)]; e++);
      if (e == end)
	 break;
      count++;
      if (count > num && num != 0)
	 break;

      if (flag)
	 *e = _clip_uptbl[*e];
      else
	 *e = _clip_lowtbl[*e];

   }
   free(buf);
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l1);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l1);
   return 0;
}
