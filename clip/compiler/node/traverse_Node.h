int
traverse_Node(Node * node, int (*func) (Node * np, void *par), void *par)
{
#if 0
   Node     *p, *p0;

   for (p0 = p = (Node *) node->list.head; p;)
    {
       int       r;

       r = func(p, par);
       if (r)
	  return r;

       p = (Node *) p->listel.next;
       if (p == p0)
	  break;
    }

   return 0;
#endif
   TraversePar tpar;

   if (!node)
      return 1;
   tpar.func = func;
   tpar.par = par;

   return node->pass(node, Traverse, 0, &tpar);

}
