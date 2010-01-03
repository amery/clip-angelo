Node     *
new_MemvarNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_MemvarNode, "memvar");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isMemvar = 1;
	curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}
