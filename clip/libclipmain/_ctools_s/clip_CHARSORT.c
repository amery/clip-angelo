int
clip_CHARSORT(ClipMachine * ClipMachineMemory)
{
   int l, i, nsort, epos;

   int *set;

   unsigned char *ret, *beg, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int elen = _clip_parni(ClipMachineMemory, 2);

   int clen = _clip_parni(ClipMachineMemory, 3);

   int ignore = _clip_parni(ClipMachineMemory, 4);

   int bpos = _clip_parni(ClipMachineMemory, 5);

   int slen = _clip_parni(ClipMachineMemory, 6);

   int flag = _clip_parl(ClipMachineMemory, 7);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARSORT");
   }
   for (i = 2; i < 7; i++)
   {
      if (_clip_parinfo(ClipMachineMemory, i) != LOGICAL_type_of_ClipVarType)
	 continue;
      flag = _clip_parl(ClipMachineMemory, i);
      break;
   }
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   if (elen <= 0)
      elen = 1;
   if (bpos <= 0)
      bpos = 0;
   if (clen <= 0)
      clen = elen - bpos;
   if ((clen + bpos) > elen)
      clen = elen - bpos;
   epos = bpos + clen;
   if (_clip_parinfo(ClipMachineMemory, 6) != NUMERIC_type_of_ClipVarType)
      slen = l;
   if (slen <= 0)
      slen = 0;
   if (ignore <= 0)
      ignore = 0;
   beg = ret + ignore;
   end = beg + slen;
   end -= ((end - beg) % elen);
   if (end > (ret + l))
      end = ret + l;
   if (elen > l || ignore > l || beg == end)
   {
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
      return 0;
   }
   nsort = (end - beg) / elen;
   set = _clip_fetch_item(ClipMachineMemory, HASH_charsort_param);
   set[0] = flag;
   set[1] = elen;
   set[2] = bpos;
   set[3] = epos;
   _clip_charsort_mp = ClipMachineMemory;
   qsort(beg, nsort, elen, _clip_charsort);
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
