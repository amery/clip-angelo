Node *
new_MemvarFNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_FMemvarNode, "fmemvar");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isFMemvar = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);
   ret->p1 = 1;

   return (Node *) ret;
}
