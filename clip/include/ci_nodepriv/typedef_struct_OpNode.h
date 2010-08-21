typedef struct OpNode
{
   Node node;
   Node *expr1;
   Node *expr2;
   int op;
}
OpNode;
