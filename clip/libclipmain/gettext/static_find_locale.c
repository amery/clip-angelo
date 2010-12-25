static Locale *
find_locale(char *module)
{
   int ind, i;

   Locale l, *lp;

   l.module = module;

   for (i = 0; i < 2; i++)
   {
      if (search_Coll(&locales, &l, &ind))
      {
			lp = (Locale *) locales.items_of_Coll[ind];
	 if (lp->ok)
	    return lp;
	 else
	    return 0;
      }
      add_locale(module, 0);
   }

   return 0;
}
