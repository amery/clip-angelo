static int
pass_ExprListNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprListNode, npp, self);

   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "ExprListNode: isLast %d\n", npp->last);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  VAR(Node, np, self);
	  int       r, n = 0;

	  for (r = first_List(&np->list); r; r = next_List(&np->list))
	   {
	      VAR(Node, p, np->list.current);
	      if (n)
		 putString_StrBuf(out, ", ");
	      n++;
	      if (p->pass(p, pass, level + 1, par))
		 break;
	   }
       }
       return 0;
    case Traverse:
       {
	  VAR(TraversePar, tp, par);
	  tp->func((Node *) self, tp->par);
       }
       break;
    case CText:
       {
	  int       r;

	  VAR(FILE, out, par);
	  VAR(Node, np, self);
	  for (r = first_List(&np->list); r; r = next_List(&np->list))
	   {
	      VAR(Node, p, np->list.current);
	      p->pass(p, pass, level + 1, par);
	      if (p->listel.next != np->list.head && npp->last)
		 fprintfOffs(out, level + 1, "_clip_pop(_mp);\n");
	   }

       }
       return 0;
    case OText:
       {
	  int       r;

	  VAR(StrBuf, out, par);
	  VAR(Node, np, self);
	  for (r = first_List(&np->list); r; r = next_List(&np->list))
	   {
	      VAR(Node, p, np->list.current);
	      p->pass(p, pass, level + 1, par);
	      if (p->listel.next != np->list.head && npp->last)
		 putByte_StrBuf(out, CLIP_POP);
	   }

       }
       return 0;
    default:
       break;
    }
   return pass_Node(self, pass, level - 1, par);
}
