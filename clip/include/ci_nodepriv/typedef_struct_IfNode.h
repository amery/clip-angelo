typedef struct IfNode
{
   Node node;
   Coll *elseifs;
   Node *elselist;
   int labelNo;
}
IfNode;
