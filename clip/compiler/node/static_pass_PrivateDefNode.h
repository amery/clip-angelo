static int
pass_PrivateDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
    {
    case Print:
       {
	  int       i, count = np->cp->unsorted.count;

	  fprintfOffs(stdout, level, "private def node: %d vars\n", count);
	  for (i = 0; i < count; i++)
	   {
	      VAR(Var, vp, np->cp->unsorted.items[i]);
	      print_Var(vp, level + 1);
	   }
       }
       break;
    case Traverse:
       {
	  VAR(TraversePar, tp, par);
	  tp->func((Node *) self, tp->par);
       }
       break;
    default:
       break;
    }

   return pass_Node(np, pass, level, par);
}
