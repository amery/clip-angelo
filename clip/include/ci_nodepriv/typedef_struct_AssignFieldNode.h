typedef struct AssignFieldNode
{
   Node node;
   char *area;
   Node *areaExpr;
   char *name;
   Node *nameExpr;
   Node *expr;
}
AssignFieldNode;
