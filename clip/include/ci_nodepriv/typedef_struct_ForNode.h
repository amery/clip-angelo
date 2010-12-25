typedef struct ForNode
{
   Node node;
   Node *var;
   Node *init;
   Node *to;
   Node *step;
   Node *list;
   int labelNo;
   int keys;
}
ForNode;
