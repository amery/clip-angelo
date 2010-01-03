int
getCount_Node(void *node)
{
   VAR(Node, np, node);
   int       r, ret = 0;

   for (r = first_List(&np->list); r; r = next_List(&np->list))
      ret++;
   return ret;
}
