Node     *
new_CodestrNode(Node * expr, int block, VarColl * params)
{
   int       i;

   NEWVAR(CodestrNode, ret);

   init_Node(ret, pass_CodestrNode, "codestr");

   ret->expr = expr;
   ret->node.isExpr = 1;
   ret->buf = new_StrBuf();
   ret->block = block;
   ret->params = params;
   init_Coll(&ret->coll, 0, 0);

   if (params)
      for (i = 0; i < params->unsorted.count; i++)
       {
	  VAR(Var, vpp, params->unsorted.items[i]);
	  vpp->isCodeParam = 1;
	  vpp->isParam = 1;
       }

   return (Node *) ret;
}
