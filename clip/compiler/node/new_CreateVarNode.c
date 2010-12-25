Node *
new_CreateVarNode(Var * vp, long space, char *spacename)
{
   NEWVAR(CreateVarNode, ret);

   ins_name(spacename);
   ins_name(vp->name);
   init_Node(ret, pass_CreateVarNode, "createvar");
   ret->vp = vp;
   ret->space = space;
   ret->spacename = spacename;

   return (Node *) ret;
}
