static int
depth_search(Coll * graph, DepNode * start, int *setno)
{
   int i;

   int find = 0;

   start->setno = -3;
   for (i = 0; i < start->ndeps; i++)
   {
      DepNode *np = find_node(graph, start->deps[i]);

      if (!np)
	 continue;
      if (np->in_cycle)
	 continue;
      switch (np->setno)
      {
      case -1:
	 /* unprocessed node */
	 depth_search(graph, np, setno);
	 find++;
	 break;
      case -3:
	 /* cycle detected */
	 set_cycle(graph, np, np);
	 break;
      default:
	 /* procecced node */
	 break;
      }
   }

   if (!find)			/* terminate node */
      start->setno = (*setno)++;
   else				/* rollback */
      start->setno = -1;

   return find;
}
