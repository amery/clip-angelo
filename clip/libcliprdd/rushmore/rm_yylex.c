int
rm_yylex(RDD_DATA * rd, int nomove)
{
   int r = 0;

   char c = *rd->ptr;

   char quote = 0;

   int ps = 0;

   char *oldptr = rd->ptr;

   if (!c)
      return RM_END_EXPR;

   if (rd->yylval)
   {
      free(rd->yylval);
      rd->yylval = NULL;
   }

   if (c != '\'' && c != '"' && c != '[')
      while (*(++rd->ptr) == ' ');
   else
      rd->ptr++;
   while (!r)
   {
      if (quote)
      {
	 if (c == quote)
	 {
	    quote = 0;
/*				while(*(rd->ptr)==' ') rd->ptr++;
				c = *rd->ptr;
				rd->ptr++;*/
	 }
      }
      else
      {
	 if (c == '"' || c == '\'')
	    quote = c;
      }
      if (c == RM_END_EXPR)
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else
	 {
	    r = RM_ERROR;
	 }
      }
      else if (!quote && (c == RM_LP))
      {
	 r = RM_LP;
      }
      else if (!quote && (c == RM_RP))
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else
	 {
	    r = RM_RP;
	 }
      }
      else if (!quote && (c == '.' && isalpha(*rd->ptr) && *(rd->ptr + 1) != '.'))
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else if (strncasecmp(rd->ptr, "OR.", 3) == 0)
	 {
	    r = RM_OR;
	    rd->ptr += 3;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else if (strncasecmp(rd->ptr, "AND.", 4) == 0)
	 {
	    r = RM_AND;
	    rd->ptr += 4;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else if (strncasecmp(rd->ptr, "NOT.", 4) == 0)
	 {
	    r = RM_NOT;
	    rd->ptr += 4;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
      }
      else if (!quote && (c == '=') && !ps)
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else if (*rd->ptr == '=')
	 {
	    r = RM_EEQU;
	    rd->ptr++;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else
	 {
	    r = RM_EQU;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
      }
      else if (!quote && (c == '>') && !ps)
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else if (*rd->ptr == '=')
	 {
	    r = RM_LARE;
	    rd->ptr++;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else
	 {
	    r = RM_LAR;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
      }
      else if (!quote && (c == '<') && !ps)
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else if (*rd->ptr == '=')
	 {
	    r = RM_LESE;
	    rd->ptr++;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else if (*rd->ptr == '>')
	 {
	    r = RM_NEQU;
	    rd->ptr++;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else
	 {
	    r = RM_LES;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
      }
      else if (!quote && (c == '!') && !ps)
      {
	 if (rd->word)
	 {
	    rd->word = realloc(rd->word, rd->wlen + 1);
	    rd->word[rd->wlen] = 0;
	    rd->yylval = strdup(rd->word);
	    r = RM_WORD;
	    free(rd->word);
	    rd->word = NULL;
	    while (*(--rd->ptr) == ' ');
	 }
	 else if (*rd->ptr == '=')
	 {
	    r = RM_NEQU;
	    rd->ptr++;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
	 else
	 {
	    r = RM_NOT;
	    while (*rd->ptr == ' ')
	       rd->ptr++;
	 }
      }
      else
      {
	 if (c == '(')
	    ps++;
	 else if (c == ')')
	    ps--;
	 if (!rd->word)
	 {
	    rd->word = malloc(1);
	    rd->wlen = 1;
	 }
	 else
	 {
	    rd->wlen++;
	    rd->word = realloc(rd->word, rd->wlen);
	 }
	 rd->word[rd->wlen - 1] = c;
	 r = 0;
	 c = *rd->ptr;
	 rd->ptr++;
      }
   }
   if (nomove)
      rd->ptr = oldptr;
   return r;
}
