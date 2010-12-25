static int
find_paramsonly(Node * np, void *par)
{
   VAR(int, ip, par);

   if (!strcmp(np->name, "macro"))
      np = ((ExprNode *) np)->expr;

   if (!strcmp(np->name, "local"))
   {
      if (((VarNode *) np)->vp->isCodeParam)
	 return 0;
   }
   else if (!strcmp(np->name, "const")	/* bug#55?? */
	    || !strcmp(np->name, "arrel") || !strcmp(np->name, "call"))
      return 0;

   if (!np->isExpr)
      return 0;

   if (!np->isLval)
      return 0;

   *ip = 0;
   return 0;
}
