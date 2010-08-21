static void
check_names(Coll * ex, Coll * nm)
{
   int i;

   char *s;

   if (!names_flag)
      return;

	for (i = 0; i < ex->count_of_Coll; i++)
   {
      int no;

		s = (char *) ex->items_of_Coll[i];
      if (!search_Coll(nm, s, &no))
	 yywarning("possible not defined function %s", s);
   }
}
