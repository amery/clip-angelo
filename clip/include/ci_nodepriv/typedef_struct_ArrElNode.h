typedef struct ArrElNode
{
   Node node;
   Node *arr;
   int dim;
   Node *expr;
   int op;
   Node *oarr;
}
ArrElNode;
