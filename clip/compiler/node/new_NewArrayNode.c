Node *
new_NewArrayNode(Coll * cp)
{
   NEWVAR(NewArrayNode, ret);

   init_Node(ret, pass_NewArrayNode, "newarray");
   ret->node.isExpr = 1;

   ret->cp = cp;
   ret->node.isArray = 1;

   return (Node *) ret;
}
