Node *
new_LocalNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_LocalNode, "local");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isLocal = 1;
   ret->fp = curFunction;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}
