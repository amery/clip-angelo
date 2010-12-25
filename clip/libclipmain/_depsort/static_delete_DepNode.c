static void
delete_DepNode(void *Np)
{
   DepNode *np;

   if (!Np)
      return;
   np = (DepNode *) Np;

   if (np->deps)
      free(np->deps);

   free(np);
}
