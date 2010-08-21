typedef struct ObjNode
{
   Node node;
   char *name;
   long hash;
   int argc;
   Node *obj;
   int rest;
}
ObjNode;
