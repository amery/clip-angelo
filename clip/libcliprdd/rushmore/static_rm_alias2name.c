static char *
rm_alias2name(int npseudo, RDD_PSEUDO * pseudo, char *alias)
{
   int i;

   for (i = 0; i < npseudo; i++)
   {
      if (strcasecmp(alias, pseudo[i].alias) == 0)
	 return strdup(pseudo[i].name);
   }
   return strdup(alias);
}
