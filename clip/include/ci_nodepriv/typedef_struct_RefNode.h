typedef struct RefNode
{
   Node node;
   Node *expr;
   char *name;
   Function *fp;
   int selfref;
}
RefNode;
