int
clip_SPLIT(ClipMachine * ClipMachineMemory)
{
   int sl, length, i;

   unsigned char *string = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &length);	/* string */

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &sl);	/* pattern */

   unsigned char *str, *buf;

   unsigned char *ustr;

   int start = 0;		/*from (start position) */

   int range = length;		/*range */

   int b;

   long l[2];

   ClipVar *rg = RETPTR(ClipMachineMemory);

   regex_t preg;

   regmatch_t rmatch[RE_NREGS];

   if (s == NULL || string == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SEARCH");
   }

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
   ustr = string;
   for (i = 0; i < range; i++, string++)
   {
      if (*(string + start) > 127)
	 str[i] = _clip_cmptbl[*(string + start)];

   }
   memset(&preg, 0, sizeof(preg));

   regcomp(&preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   l[0] = 0;
   _clip_array(ClipMachineMemory, rg, 1, l);

   b = 0;
   while (b <= range)
   {
      if (regexec(&preg, (const char *) (str + b), RE_NREGS, rmatch, 0 /*REG_NOTBOL */ ) == 0)
      {
	 int j = 0;

	 ClipVar *st;

	 j = rmatch[0].rm_so;
	 if (j == -1)
	 {
	    st = NEW(ClipVar);

	    j = range - b;
	    st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	    st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	    memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	    st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	    st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	    _clip_aadd(ClipMachineMemory, rg, st);

	    _clip_delete(ClipMachineMemory, st);
	    break;
	 }

	 st = NEW(ClipVar);

	 st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	 memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	 _clip_aadd(ClipMachineMemory, rg, st);

	 _clip_delete(ClipMachineMemory, st);

	 b += rmatch[0].rm_eo;
      }
      else
      {
	 ClipVar *st;

	 int j;

	 st = NEW(ClipVar);

	 j = range - b;
	 st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	 memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	 st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	 _clip_aadd(ClipMachineMemory, rg, st);

	 _clip_delete(ClipMachineMemory, st);
	 break;
      }
   }				// while

   regfree(&preg);
   free(str);
   free(buf);

   return 0;
}
