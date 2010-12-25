int
clip_ATREPL(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, l3, sovp = 0, rcur, atlike0, atlike1;

   unsigned char *ret, *cur, *s, *e1, *e2, *end, *send, *buf;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   unsigned char *rstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l3);

   int count = _clip_parni(ClipMachineMemory, 4);

   int flag = _clip_parl(ClipMachineMemory, 5);

   char *atlike = _clip_fetch_item(ClipMachineMemory, HASH_setatlike);

   int cset = (*(unsigned char *) _clip_fetch_item(ClipMachineMemory, HASH_csetatmupa)) == 't';

   int rset = (*(unsigned char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   atlike0 = atlike[0];
   atlike1 = atlike[1];

   if (sstr == NULL || str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ATREPL");
   }
   /* check recursively at */
   for (s = rstr, end = rstr + l3; s < end; s++)
   {
      sovp = 0;
      for (cur = sstr, e1 = s, send = sstr + l1; cur < send && e1 < end; cur++, e1++)
	 if (*e1 == *cur)
	    sovp++;
      if (sovp == l1)
      {
	 cset = 0;
	 break;
      }
   }
   if (count < 0)
      count = 0;
   buf = str;
   if (l1 == 0)
   {
      _clip_retcn(ClipMachineMemory, (char *) str, l2);
      return 0;
   }
   while (1)
   {
      l = 0;
      ret = malloc(l + 1);
      e1 = buf;
      cur = buf;
      end = buf + l2;
      send = sstr + l1;
      for (sovp = 0, rcur = 0; e1 < end; e1++)
      {
	 if (*e1 != *sstr)
	    continue;
	 for (s = sstr, e2 = e1; s < send && e2 < end && (*s == *e2 || (atlike0 && *s == atlike1)); s++, e2++);
	 if (s != send)
	    continue;
	 sovp++;
	 if (flag && sovp != count)
	    continue;
	 l += e1 - cur + l3;
	 ret = realloc(ret, l + 1);
	 memcpy(ret + rcur, cur, e1 - cur);
	 rcur += e1 - cur;
	 memcpy(ret + rcur, rstr, l3);
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
      ret = realloc(ret, l + 1);
      memcpy(ret + rcur, cur, e1 - cur);
      ret[l] = 0;
      if (buf != str)
	 free(buf);
      if (!cset || sovp == 0 || flag || count != 0)
	 break;
      buf = malloc(l + 1);
      memcpy(buf, ret, l);
      l2 = l;
      free(ret);
   }
   if (rset && _clip_par_isref(ClipMachineMemory, 2))
      _clip_par_assign_str(ClipMachineMemory, 2, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
