Node *
new_FieldNode(Var * vp)
{
   NEWVAR(FieldNode, ret);

   init_Node(ret, pass_FieldNode, "field");
   ret->node.isExpr = 1;
   /*ret->node.isLval=1; */
   ret->node.isField = 1;
   ins_name(vp->name);

   ret->name = vp->name;
   ret->area = vp->alias;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
