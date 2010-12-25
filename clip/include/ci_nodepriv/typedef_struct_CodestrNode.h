typedef struct CodestrNode
{
   Node node;
   Node *expr;
   StrBuf *buf;
   Coll coll;
   VarColl *params;
   int block;
}
CodestrNode;
