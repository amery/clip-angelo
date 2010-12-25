int
pass_Node(void *self, Pass pass, int level, void *par)
{
   int r;

   VAR(Node, np, self);
   for (r = first_List(&np->list); r; r = next_List(&np->list))
   {
		VAR(Node, p, np->list.current_of_List);
      if (p->pass(p, pass, level + 1, par))
	 break;
   }

   return 0;
}
