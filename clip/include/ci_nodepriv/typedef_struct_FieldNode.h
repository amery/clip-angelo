typedef struct FieldNode
{
   Node node;
   char *name;
   char *area;
   Node *nameExpr;
   Node *areaExpr;
}
FieldNode;
