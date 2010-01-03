static void
split_vector(char *str, char ***vpp, int *lenp)
{
   int       l;

   char     *tok;

   while ((tok = strtok(str, " \t")))
    {
       str = 0;
       if (!*tok)
	  continue;
       l = (*lenp)++;
       *vpp = (char **) realloc(*vpp, (l + 1) * sizeof(char *));

       (*vpp)[l] = tok;
    }
}
