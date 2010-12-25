static int
put_env(char ***envp, char *name)
{
   char *s;

   char **ep;

   int l, len, ln;

   ln = strcspn(name, "=");
   for (ep = *envp, len = 0; ep && (s = *ep); ++ep, ++len)
   {
      l = strcspn(s, "=");
      if (ln == l && !memcmp(name, s, l))
      {
	 /* owerwrite */
	 free(s);
	 *ep = strdup(name);
	 return 1;
      }
   }

   *envp = (char **) realloc(*envp, (len + 2) * sizeof(char *));

   (*envp)[len] = strdup(name);
   (*envp)[len + 1] = 0;

   return 0;
}
