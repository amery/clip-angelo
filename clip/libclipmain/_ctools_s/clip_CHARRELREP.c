int
clip_CHARRELREP(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, l3, l4, l5, cpos;

   unsigned char *e1, *e2, *e3 = NULL, *e4;

   unsigned char *ee1, *ee3, *cur, *ret;

   unsigned char *end1, *end2, *end3, *end4;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   unsigned char *str3 = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l3);

   unsigned char *str4 = (unsigned char *) _clip_parcl(ClipMachineMemory, 4, &l4);

   unsigned char *str5 = (unsigned char *) _clip_parcl(ClipMachineMemory, 5, &l5);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str1 == NULL || str2 == NULL || str3 == NULL || str4 == NULL || str5 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARRELREP");
   }
   if (l1 == 0 || l3 == 0)
   {
      _clip_retcn(ClipMachineMemory, (char *) str4, l4);
      return 0;
   }
   l = 0;
   cpos = 0;
   ret = malloc(l + 1);
   cur = str4;
   end1 = str1 + l1;
   end3 = str3 + l3;
   end2 = str2 + l2 - l1 + 1;
   end4 = str4 + l4 - l3 + 1;
   for (e2 = str2, e4 = str4; e2 < end2 && e4 < end4; e2++, e4++)
   {
      for (e1 = str1, ee1 = e2; e1 < end1 && *e1 == *ee1; e1++, ee1++);
      if (e1 == end1)
	 for (e3 = str3, ee3 = e4; e3 < end3 && *e3 == *ee3; e3++, ee3++);
      if (e1 == end1 && e3 == end3)
      {
	 l += e4 - cur + l5;
	 ret = realloc(ret, l + 1);
	 memcpy(ret + cpos, cur, e4 - cur);
	 cpos += e4 - cur;
	 memcpy(ret + cpos, str5, l5);
	 cpos += l5;
	 e2 += l3;
	 e4 += l3;
	 cur = e4;
      }

   }
   l += str4 + l4 - cur;
   ret = realloc(ret, l + 1);
   memcpy(ret + cpos, cur, str4 + l4 - cur);
   ret[l] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 4))
      _clip_par_assign_str(ClipMachineMemory, 4, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
