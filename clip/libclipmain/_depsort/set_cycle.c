void
set_cycle(Coll * graph, DepNode * node, DepNode * end)
{
   int i;

   if (node->in_cycle)
      return;
   node->in_cycle = 1;
   for (i = 0; i < node->ndeps; i++)
   {
      DepNode *np = find_node(graph, node->deps[i]);

      if (np && np != end)
	 set_cycle(graph, np, end);
   }
}
