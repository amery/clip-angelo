int
load_charset(FILE * mapf, cons_CharsetEntry ** cp, int *lenp)
{
   int r = -1;

   char buffer[1024];

   int i;

   int fp0, fp1;

   long un0, un1;

   char *p, *p1;

   int line = 0;

   if (!mapf)
      goto ret;

   while (fgets(buffer, sizeof(buffer), mapf) != NULL)
   {
      line++;
      if ((p = strchr(buffer, '\n')) != NULL)
	 *p = '\0';
      else
	 fprintf(stderr, "load_charset: Warning: line %d too long\n", line);

      p = buffer;

/*
 * Syntax accepted:
 *      <fontpos>       <unicode> <unicode> ...
 *      <range>         idem
 *      <range>         <unicode range>
 *
 * where <range> ::= <fontpos>-<fontpos>
 * and <unicode> ::= U+<h><h><h><h>
 * and <h> ::= <hexadecimal digit>
 */

      while (*p == ' ' || *p == '\t')
	 p++;
      if (!*p || *p == '#')
	 continue;		/* skip comment or blank line */

      fp0 = strtol(p, &p1, 0);
      if (p1 == p)
      {
	 fprintf(stderr, "Bad input line: %s\n", buffer);
	 goto ret;
      }
      p = p1;

      while (*p == ' ' || *p == '\t')
	 p++;
      if (*p == '-')
      {
	 p++;
	 fp1 = strtol(p, &p1, 0);
	 if (p1 == p)
	 {
	    fprintf(stderr, "Bad input line: %s\n", buffer);
	    goto ret;
	 }
	 p = p1;
      }
      else
	 fp1 = 0;

      if (fp0 < 0 || fp0 > 65535)
      {
	 continue;
      }
      if (fp1 && (fp1 < fp0 || fp1 > 65535))
      {
	 fprintf(stderr, "line %d: Bad end of range (0x%x)\n", line, fp1);
	 goto ret;
      }

      if (fp1)
      {
	 /* we have a range; expect the word "idem" or a Unicode range of the
	    same length */
	 while (*p == ' ' || *p == '\t')
	    p++;
	 if (!strncmp(p, "idem", 4))
	 {
	    for (i = fp0; i <= fp1; i++)
	       addpair(i, i, cp, lenp);
	    p += 4;
	 }
	 else
	 {
	    un0 = getunicode(&p);
	    while (*p == ' ' || *p == '\t')
	       p++;
	    if (*p != '-')
	    {
	       fprintf(stderr, "line %d: Corresponding to a range of font positions, " "there should be a Unicode range\n", line);
	       goto ret;
	    }
	    p++;
	    un1 = getunicode(&p);
	    if (un0 < 0 || un1 < 0)
	    {
	       fprintf(stderr, "line %d: Bad Unicode range corresponding to " "font position range 0x%x-0x%x\n", line, fp0, fp1);
	       goto ret;
	    }
	    if (un1 - un0 != fp1 - fp0)
	    {
	       fprintf(stderr, "line %d: Unicode range U+%lx-U+%lx not of the same length" "as font position range 0x%x-0x%x\n", line, un0, un1, fp0, fp1);
	       goto ret;
	    }
	    for (i = fp0; i <= fp1; i++)
	       addpair(i, un0 - fp0 + i, cp, lenp);
	 }
      }
      else
      {
	 /* no range; expect a list of unicode values
	    for a single font position */

	 while ((un0 = getunicode(&p)) >= 0)
	    addpair(fp0, un0, cp, lenp);
      }
      while (*p == ' ' || *p == '\t')
	 p++;
      if (*p && *p != '#')
	 fprintf(stderr, "line %d: trailing junk (%s) ignored\n", line, p);
   }

   r = 0;
 ret:

   return r;
}
