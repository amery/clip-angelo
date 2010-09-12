static void
print_Constants(Coll * cp, char *msg)
{
   int i, count;

	count = cp->count_of_Coll;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
   {
		VAR(ConstNode, s, cp->items_of_Coll[i]);
      printf("\t'%s'\n", s->val);
   }
}
