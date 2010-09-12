Node *
new_AssignNode(Node * var, Node * expr, int op)
{
   AssignNode *ret;

   if (var->isField)
   {
      FieldNode *fp = (FieldNode *) var;

      if (op != '=')
      {
	 return new_AssignFieldNode(fp->area, fp->areaExpr, fp->name, fp->nameExpr, new_OpNode(var, expr, op));
      }
      else
      {
	 return new_AssignFieldNode(fp->area, fp->areaExpr, fp->name, fp->nameExpr, expr);
      }
   }

   if (!var->isLval)
      yyerror("assign to non-lvalue");

   if (var->isMacro && expr->isRef)
      yyerror("reference to macro assign not allowed");

   if (op == '=' && field_flag && !strcmp(var->name, "fmemvar"))
   {
      return new_AssignFmemvarNode(var, expr);
   }
   var->isAssignLval = 1;

   ret = NEW(AssignNode);
   init_Node(ret, pass_AssignNode, "assign");
   ret->node.isExpr = 1;

   ret->var = var;
   ret->expr = expr;
   ret->op = op;
   ret->node.isAssign = 1;
   ret->field_flag = field_flag;

   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
