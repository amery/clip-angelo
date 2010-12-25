void
print_Var(Var * vp, int level)
{
   fprintfOffs(stdout, level, "");
   if (vp->alias)
      printf("%s->", vp->alias);
   printf("%s as=%s isRef=%d refvar=%d isLocalRef=%d", vp->name, vp->type ? vp->type : "undef", vp->isRef, vp->refvar ? vp->refvar->no : -1, vp->isLocalRef);
   if (vp->init)
   {
      printf(":=\n");
      vp->init->pass(vp->init, Print, level + 1, NULL);
   }
   else if (vp->arr)
   {
      int i;

		for (i = 0; i < vp->arr->count_of_Coll; i++)
			printf("[%s]", (char *) vp->arr->items_of_Coll[i]);
      printf("\n");
   }
   else
      printf("\n");
}
