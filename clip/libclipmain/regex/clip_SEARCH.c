/*
   if found return registers: {start position, end position}
   reg:={}
   search("90 asdf 12 ", "[a-z]+", reg) -->
   reg={{4, 8}}
*/
int
clip_SEARCH(ClipMachine * ClipMachineMemory)
{
   int c, sl, length, i;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);	/* pattern */

   unsigned char *string = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &length);	/* string */

   unsigned char *str, *buf;

   int start = _clip_parni(ClipMachineMemory, 4) - 1;	/*from (start position) */

   int range = _clip_parni(ClipMachineMemory, 5) - 1;	/*range */

   ClipVar *rg = _clip_par(ClipMachineMemory, 3);	/*registers */

   regex_t preg;

   regmatch_t rmatch[RE_NREGS];

   if (s == NULL || string == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SEARCH");
   }
   if (_clip_parinfo(ClipMachineMemory, 0) < 4)
      start = 0;
   if (_clip_parinfo(ClipMachineMemory, 0) < 5)
      range = length;
   if (range < 0)
      start += range + 1;
   if (start < 0)
      start = 0;
   range = abs(range) < length ? abs(range) : length;

   str = (unsigned char *) malloc(range + 1);
   memcpy(str, string + start, range);
   str[range] = 0;

   buf = (unsigned char *) malloc(sl + 1);
   memcpy(buf, s, sl);
   buf[sl] = 0;

   for (i = 0; i < sl; i++, s++)
   {
      if ((*s) > 127)
	 buf[i] = _clip_cmptbl[(*s)];
   }
   for (i = 0; i < range; i++, string++)
   {
      if (*(string + start) > 127)
	 str[i] = _clip_cmptbl[*(string + start)];

   }
   memset(&preg, 0, sizeof(preg));

   regcomp(&preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   if (regexec(&preg, (const char *) str, RE_NREGS, rmatch, 0 /*REG_NOTBOL */ ) == 0)
   {
      int j;

      for (j = 0; j < RE_NREGS; j++)
      {

	 ClipVar *st;

	 ClipVar *ed;

	 ClipVar *el;

	 if ((_clip_parinfo(ClipMachineMemory, 0) < 3) || rmatch[j].rm_so == -1)
	    break;

	 st = NEW(ClipVar);
	 ed = NEW(ClipVar);
	 el = NEW(ClipVar);

	 el->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	 el->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

	 st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 st->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 st->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 st->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_so + 1 + start;

	 c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	 memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	 _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, st);

	 ed->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 ed->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ed->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 ed->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_eo + 1 + start;

	 c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	 memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	 _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, ed);
	 c = ++rg->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	 memset(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	 _clip_dup(ClipMachineMemory, rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, el);

	 _clip_delete(ClipMachineMemory, st);
	 _clip_delete(ClipMachineMemory, ed);
	 _clip_delete(ClipMachineMemory, el);

      }
      if (_clip_parinfo(ClipMachineMemory, 0) > 2)
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rg);

      _clip_retl(ClipMachineMemory, 1);

   }
   else
      _clip_retl(ClipMachineMemory, 0);
   regfree(&preg);
   free(str);
   free(buf);
   return 0;
}
