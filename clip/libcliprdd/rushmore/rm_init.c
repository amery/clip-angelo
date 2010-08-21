int
rm_init(ClipMachine * ClipMachineMemory, RDD_DATA * rd, char *str, const char *__PROC__)
{
   char *end = str + strlen(str);

   int squote = 0;

   int dquote = 0;

   int ps = 0;

   for (rd->ptr = str; rd->ptr < end; rd->ptr++)
   {
      if ((*rd->ptr == '\"') && !squote)
      {
	 dquote = !dquote;
      }
      else if ((*rd->ptr == '\'') && !dquote)
      {
	 squote = !squote;
      }
      if (squote || dquote)
	 continue;
      if (*rd->ptr == '(')
      {
	 if (!ps)
	 {
	    char *c = rd->ptr;

	    while (--c >= str && *c == ' ');
	    rm_checkpar(rd->ptr, end, !(c < str || *c == '.' || *c == '!' || *c == '('));
	    if (*rd->ptr == RM_LP)
	       ps++;
	 }
	 else
	 {
	    *rd->ptr = RM_LP;
	    ps++;
	 }
      }
      else if (*rd->ptr == RM_RP)
      {
	 ps--;
      }
      else if (*rd->ptr == ')' && ps)
      {
	 *rd->ptr = RM_RP;
	 ps--;
      }
   }
   dquote = 0;
   for (rd->ptr = str; rd->ptr < end; rd->ptr++)
   {
      if (!dquote)
      {
	 if (*rd->ptr == '\"' || *rd->ptr == '\'')
	    dquote = *rd->ptr;
      }
      else if (*rd->ptr == dquote)
      {
	 dquote = 0;
      }
      if (dquote)
	 continue;
      if (*rd->ptr == '(')
	 *rd->ptr = RM_LP;
      else if (*rd->ptr == ')')
	 *rd->ptr = RM_RP;
      else if (*rd->ptr == RM_LP)
	 *rd->ptr = '(';
      else if (*rd->ptr == RM_RP)
	 *rd->ptr = ')';
      else if (*rd->ptr == '>' && rd->ptr != str && *(rd->ptr - 1) == '-')
	 *rd->ptr = RM_AL;
   }
   rd->ptr = str;
   rd->word = 0;
   rd->wlen = 0;
   return 0;
}
