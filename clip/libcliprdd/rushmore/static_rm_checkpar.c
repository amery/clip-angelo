static void
rm_checkpar(char *c, char *end, int func)
{
   int ps = 1;

   char *b = c;

   int squote = 0;

   int dquote = 0;

   int comma = 0;

   while (ps && (++c < end))
   {
      if ((*c == '\"') && !squote)
      {
	 dquote = !dquote;
      }
      else if ((*c == '\'') && !dquote)
      {
	 squote = !squote;
      }
      if (squote || dquote)
	 continue;
      if (*c == ')')
      {
	 ps--;
      }
      else if (*c == '(')
      {
	 ps++;
      }
      else if (*c == ',' && ps == 1)
      {
	 comma = 1;
      }
      else if ((*c == '<' || (*c == '>' && *(c - 1) != '-') || *c == '!' || *c == '=' || (*c == '.' && isalpha(*(c + 1)) && isalpha(*(c + 2)))) && !func)
      {
	 return;
      }
   }
   if (!ps)
   {
      (*b) = RM_LP;
      (*c) = RM_RP;
   }
}
