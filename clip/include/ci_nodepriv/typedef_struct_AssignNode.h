typedef struct AssignNode
{
   Node node;
   Node *var;
   Node *expr;
   int op;
   int field_flag;
}
AssignNode;
