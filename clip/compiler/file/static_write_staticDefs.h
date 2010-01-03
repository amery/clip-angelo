static void
write_staticDefs(FILE * out, VarColl * statics)
{
   int       j;

   for (j = 0; j < statics->coll.count; j++)
    {
       VAR(Var, vp, statics->coll.items[j]);
		 fprintf(out, "\t{ %ld, &s_%s_%s },\n", (long) hashstr(vp->name), vp->func->name_of_Function, vp->name);
    }
}
