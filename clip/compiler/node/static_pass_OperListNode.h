static int
pass_OperListNode(void *self, Pass pass, int level, void *par)
{
   VAR(Node, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "operlist node:\n");
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
