int
clip_WORDREPL(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, l3, sovp = 0, rcur;

   short *ret, *cur, *s, *e1, *e2, *end, *send, *buf;

   short *sstr = (short *) _clip_parcl(ClipMachineMemory, 1, &l1);

   short *str = (short *) _clip_parcl(ClipMachineMemory, 2, &l2);

   short *rstr = (short *) _clip_parcl(ClipMachineMemory, 3, &l3);

   int flag = _clip_parl(ClipMachineMemory, 4);

   int count = _clip_parni(ClipMachineMemory, 5);

   int cset = (*(unsigned char *) _clip_fetch_item(ClipMachineMemory, HASH_csetatmupa)) == 't';

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (sstr == NULL || str == NULL || rstr == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "WORDREPL");
   }
   if (count < 0)
      count = 0;
   buf = str;
   if (l1 == 0)
   {
      _clip_retcn(ClipMachineMemory, (char *) str, l2);
      return 0;
   }
   l1 = l1 >> 1;
   l2 = l2 >> 1;
   l3 = l3 >> 1;
   while (1)
   {
      l = 0;
      ret = malloc((l + 1) * 2);
      e1 = buf;
      cur = buf;
      end = buf + l2;
      send = sstr + l1;
      for (sovp = 0, rcur = 0; e1 < end; e1++)
      {
	 if (*e1 != *sstr)
	    continue;
	 for (s = sstr, e2 = e1; s < send && e2 < end && *s == *e2; s++, e2++);
	 if (s != send)
	    continue;
	 sovp++;
	 /*
	    if (flag && (sovp!=count || count==0))
	    continue;
	  */
	 l += e1 - cur + l3;
	 ret = realloc(ret, (l + 1) * 2);
	 memcpy(ret + rcur, cur, (e1 - cur) * 2);
	 rcur += e1 - cur;
	 memcpy(ret + rcur, rstr, l3 * 2);
	 rcur += l3;
	 e1 = e2 - 1;
	 cur = e2;
	 if (count != 0 && sovp == count)
	 {
	    e1 = end;
	    break;
	 }
      }
      l += e1 - cur;
      ret = realloc(ret, (l + 1) * 2);
      memcpy(ret + rcur, cur, (e1 - cur) * 2);
      ret[l] = 0;
      if (buf != str)
	 free(buf);
      if (!cset || sovp == 0 || flag || count != 0)
	 break;
      buf = malloc((l + 1) * 2);
      memcpy(buf, ret, l * 2);
      l2 = l;
      free(ret);
   }
   if (rset && _clip_par_isref(ClipMachineMemory, 2))
      _clip_par_assign_str(ClipMachineMemory, 2, (char *) ret, l * 2);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l * 2);
   return 0;
}
