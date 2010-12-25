typedef struct WhileNode
{
   Node node;
   Node *expr;
   Node *list;
   int labelNo;
}
WhileNode;
