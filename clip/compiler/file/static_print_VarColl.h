static void
print_VarColl(VarColl * cp, char *msg)
{
   int       i, count;

   count = cp->unsorted.count;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
    {
       VAR(Var, vp, cp->unsorted.items[i]);
       print_Var(vp, 1);
    }
}
