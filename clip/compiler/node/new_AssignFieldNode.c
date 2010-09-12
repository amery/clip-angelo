Node *
new_AssignFieldNode(char *area, Node * areaExpr, char *name, Node * nameExpr, Node * expr)
{
   NEWVAR(AssignFieldNode, ret);

   init_Node(ret, pass_AssignFieldNode, "assignfield");
   ret->node.isExpr = 1;

   ret->area = area;
   ret->areaExpr = areaExpr;
   ret->name = name;
   ret->nameExpr = nameExpr;
   ret->expr = expr;
   ret->node.isAssign = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(name);
   ins_name(area);

   return (Node *) ret;
}
