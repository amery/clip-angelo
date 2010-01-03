static void
print_Constants(Coll * cp, char *msg)
{
   int       i, count;

   count = cp->count;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
    {
       VAR(ConstNode, s, cp->items[i]);
       printf("\t'%s'\n", s->val);
    }
}
