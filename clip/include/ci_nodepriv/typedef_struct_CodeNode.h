typedef struct CodeNode
{
   Node node;
   Function *func;
   Function *fp;
   Coll inits;
}
CodeNode;
