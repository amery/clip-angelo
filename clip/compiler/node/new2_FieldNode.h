Node     *
new2_FieldNode(char *area, Node * areaExpr, char *name, Node * nameExpr)
{
   NEWVAR(FieldNode, ret);

   init_Node(ret, pass_FieldNode, "field");
   ret->node.isExpr = 1;
  /*ret->node.isLval = 1; */
   ret->node.isField = 1;
  /*ret->node.isMemvar = 1; */
   ins_name(area);
   ins_name(name);

   ret->name = name;
   ret->area = area;
   ret->nameExpr = nameExpr;
   ret->areaExpr = areaExpr;
	curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
