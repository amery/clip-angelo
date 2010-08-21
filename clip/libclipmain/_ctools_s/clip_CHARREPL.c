int
clip_CHARREPL(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, l3, pos, sovp;

   unsigned char *ret, *s, *e1, *end, *send;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   unsigned char *rstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l3);

   int flag = _clip_parl(ClipMachineMemory, 4);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (sstr == NULL || str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARREPL");
   }
   l = l2;
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   if (l1 == 0)
   {
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
      return 0;
   }
   if (l1 == l3 && memcmp(sstr, rstr, l1) == 0)
   {
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
      return 0;
   }
   while (1)
   {
      end = ret + l;
      send = sstr + l1;
      for (e1 = ret, sovp = 0; e1 < end; e1++)
      {
	 for (s = sstr; s < send && *s != *e1; s++);
	 if (s == send)
	    continue;
	 sovp++;
	 pos = s - sstr;
	 if (pos >= l3)
	    pos = l3 - 1;
	 *e1 = rstr[pos];

      }
      if (flag || sovp == 0)
	 break;
   }
   if (rset && _clip_par_isref(ClipMachineMemory, 2))
      _clip_par_assign_str(ClipMachineMemory, 2, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
