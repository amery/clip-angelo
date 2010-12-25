static DepNode *
find_node(Coll * graph, long hash)
{
   int ind;

   DepNode node;

   node.hash = hash;

   if (search_Coll(graph, &node, &ind))
		return (DepNode *) graph->items_of_Coll[ind];
   return 0;
}
