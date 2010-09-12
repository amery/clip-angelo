static void
print_VarColl(VarColl * cp, char *msg)
{
   int i, count;

	count = cp->unsorted.count_of_Coll;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
   {
		VAR(Var, vp, cp->unsorted.items_of_Coll[i]);
      print_Var(vp, 1);
   }
}
