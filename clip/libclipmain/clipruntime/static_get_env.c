static char *
get_env(char **envp, char *name)
{
   char *s;

   int l;

   int ln = strlen(name);

   for (; envp && (s = *envp); ++envp)
   {
      l = strcspn(s, "=");
      if (ln == l && !memcmp(name, s, l))
      {
	 s += l;
	 if (*s)
	    return s + 1;
	 else
	    return s;
      }
   }
   return 0;
}
