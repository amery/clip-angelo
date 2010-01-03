Node     *
new_ParamNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_ParamNode, "param");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->fp = curFunction;
	curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}
